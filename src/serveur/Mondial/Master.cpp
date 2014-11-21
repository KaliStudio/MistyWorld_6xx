
#include <ace/Sig_Handler.h>

#include "Common.h"
#include "SystemConfig.h"
#include "SignalHandler.h"
#include "World.h"
#include "WorldRunnable.h"
#include "WorldSocket.h"
#include "WorldSocketMgr.h"
#include "Configuration/Config.h"
#include "Database/DatabaseEnv.h"
#include "Database/DatabaseWorkerPool.h"

#include "CliRunnable.h"
#include "Log.h"
#include "Master.h"
#include "RARunnable.h"
#include "TCSoap.h"
#include "Timer.h"
#include "Util.h"
#include "AuthSocket.h"
#include "RealmList.h"

#include "BigNumber.h"
#include "OpenSSLCrypto.h"

#ifdef _WIN32
#include "ServiceWin32.h"
extern int m_ServiceStatus;
#endif

#ifdef __linux__
#include <sched.h>
#include <sys/resource.h>
#define PROCESS_HIGH_PRIORITY -15 // [-20, 19], default is 0
#endif

/// Handle Mondials's termination signals
class MondialSignalHandler : public TRINITY::SignalHandler
{
    public:
        virtual void HandleSignal(int sigNum)
        {
            switch (sigNum)
            {
                case SIGINT:
                    World::StopNow(RESTART_EXIT_CODE);
                    break;
                case SIGTERM:
#ifdef _WIN32
                case SIGBREAK:
                    if (m_ServiceStatus != 1)
#endif
                    World::StopNow(SHUTDOWN_EXIT_CODE);
                    break;
            }
        }
};

class FreezeDetectorRunnable : public ACE_Based::Runnable
{
private:
    uint32 _loops;
    uint32 _lastChange;
    uint32 _delaytime;
public:
    FreezeDetectorRunnable() { _delaytime = 0; }

    void SetDelayTime(uint32 t) { _delaytime = t; }

    void run() OVERRIDE
    {
        if (!_delaytime)
            return;

        TC_LOG_INFO("server.Mondial", "Démarrage du fil anti-freeze (%u secondes max de temps bloqué)...", _delaytime/1000);
        _loops = 0;
        _lastChange = 0;
        while (!World::IsStopped())
        {
            ACE_Based::Thread::Sleep(1000);
            uint32 curtime = getMSTime();
            // normal work
            uint32 worldLoopCounter = World::m_worldLoopCounter.value();
            if (_loops != worldLoopCounter)
            {
                _lastChange = curtime;
                _loops = worldLoopCounter;
            }
            // possible freeze
            else if (getMSTimeDiff(_lastChange, curtime) > _delaytime)
            {
                TC_LOG_ERROR("server.Mondial", "Sujet mondiale se bloque, déportant les serveur!");
                ASSERT(false);
            }
        }
        TC_LOG_INFO("server.Mondial", "Fil Anti-freeze sortie sans problèmes.");
    }
};

/// Main function
int Master::Run()
{
    OpenSSLCrypto::threadsSetup();
    BigNumber seed1;
    seed1.SetRand(16 * 8);

	TC_LOG_INFO("server.Mondial", "%s (Mondial-daemon)", _FULLVERSION);
	TC_LOG_INFO("server.Mondial", "<Ctrl-C> to stop.\n");

	TC_LOG_INFO("server.Mondial", " _____ _     _            _ _ _         _   _");
	TC_LOG_INFO("server.Mondial", "|     |_|___| |_ _ _     | | | |___ _ _| |_| |");
	TC_LOG_INFO("server.Mondial", "| | | | |_ -|  _| | |    | | | | . | '_| | . |");
	TC_LOG_INFO("server.Mondial", "|_|_|_|_|___|_| |_  |    |_____|___|_| |_|___|");
	TC_LOG_INFO("server.Mondial", "          	|___|                         ");
	TC_LOG_INFO("server.Mondial", "Projet Misty World 2014(c) Emulateur WoW");
	TC_LOG_INFO("server.Mondial", "<http://misty-world.site88.net/> \n");


    /// Mondial PID file creation
    std::string pidFile = sConfigMgr->GetStringDefault("PidFile", "");
    if (!pidFile.empty())
    {
        if (uint32 pid = CreatePIDFile(pidFile))
            TC_LOG_INFO("server.Mondial", "Daemon PID: %u\n", pid);
        else
        {
            TC_LOG_ERROR("server.Mondial", "Vous ne pouvez pas créer de fichier PID %s.\n", pidFile.c_str());
            return 1;
        }
    }

    ///- Start the databases
    if (!_StartDB())
        return 1;

    // set server offline (not connectable)
    LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = (flag & ~%u) | %u WHERE id = '%d'", REALM_FLAG_OFFLINE, REALM_FLAG_INVALID, realmID);

    ///- Initialize the World
    sWorld->SetInitialWorldSettings();

    ///- Initialize the signal handlers
    MondialSignalHandler signalINT, signalTERM;
    #ifdef _WIN32
    MondialSignalHandler signalBREAK;
    #endif /* _WIN32 */

    ///- Register Mondial's signal handlers
    ACE_Sig_Handler handle;
    handle.register_handler(SIGINT, &signalINT);
    handle.register_handler(SIGTERM, &signalTERM);
#ifdef _WIN32
    handle.register_handler(SIGBREAK, &signalBREAK);
#endif

    ///- Launch WorldRunnable thread
    ACE_Based::Thread worldThread(new WorldRunnable);
    worldThread.setPriority(ACE_Based::Highest);

    ACE_Based::Thread* cliThread = NULL;

#ifdef _WIN32
    if (sConfigMgr->GetBoolDefault("Console.Enable", true) && (m_ServiceStatus == -1)/* need disable console in service mode*/)
#else
    if (sConfigMgr->GetBoolDefault("Console.Enable", true))
#endif
    {
        ///- Launch CliRunnable thread
        cliThread = new ACE_Based::Thread(new CliRunnable);
    }

    ACE_Based::Thread rarThread(new RARunnable);

    ///- Handle affinity for multiple processors and process priority
    uint32 affinity = sConfigMgr->GetIntDefault("UseProcessors", 0);
    bool highPriority = sConfigMgr->GetBoolDefault("ProcessPriority", false);

#ifdef _WIN32 // Windows
    {
        HANDLE hProcess = GetCurrentProcess();

        if (affinity > 0)
        {
            ULONG_PTR appAff;
            ULONG_PTR sysAff;

            if (GetProcessAffinityMask(hProcess, &appAff, &sysAff))
            {
                ULONG_PTR currentAffinity = affinity & appAff;            // remove non accessible processors

                if (!currentAffinity)
                    TC_LOG_ERROR("server.Mondial", "Processeurs marqués comme utilisant des processeurs masque de bits (hex) %x ne sont pas accessibles pour le Mondial. Processeurs accessible masque de bits (hex): %x", affinity, appAff);
                else if (SetProcessAffinityMask(hProcess, currentAffinity))
                    TC_LOG_INFO("server.Mondial", "Utilisation des processeurs (bitmask, hex): %x", currentAffinity);
                else
                    TC_LOG_ERROR("server.Mondial", "Impossible de définir les processeurs utilisés (hex): %x", currentAffinity);
            }
        }

        if (highPriority)
        {
            if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
                TC_LOG_INFO("server.Mondial", "Niveau de priorité du processus mondial passe en niveau HAUT");
            else
                TC_LOG_ERROR("server.Mondial", "Impossible de définir le niveau de priorité du processus Mondial .");
        }
    }
#elif __linux__ // Linux

    if (affinity > 0)
    {
        cpu_set_t mask;
        CPU_ZERO(&mask);

        for (unsigned int i = 0; i < sizeof(affinity) * 8; ++i)
            if (affinity & (1 << i))
                CPU_SET(i, &mask);

        if (sched_setaffinity(0, sizeof(mask), &mask))
			TC_LOG_ERROR("server.Mondial", "Vous ne pouvez pas définir le processeurs utilisés (hex):%x, erreur:%s", affinity, strerror(errno));
        else
        {
            CPU_ZERO(&mask);
            sched_getaffinity(0, sizeof(mask), &mask);
            TC_LOG_INFO("server.Mondial", "Utilisation des processeurs (bitmask, hex): %x", *(uint32*)(&mask));
        }
    }

    if (highPriority)
    {
        if (setpriority(PRIO_PROCESS, 0, PROCESS_HIGH_PRIORITY))
            TC_LOG_ERROR("server.Mondial", "Impossible de définir le niveau de priorité du processus Mondial, erreur: %s", strerror(errno));
        else
            TC_LOG_INFO("server.Mondial", "Niveau de priorité du processus Mondial définie à %i", getpriority(PRIO_PROCESS, 0));
    }

#endif

    //Start soap serving thread
    ACE_Based::Thread* soapThread = NULL;

    if (sConfigMgr->GetBoolDefault("SOAP.Enabled", false))
    {
        TCSoapRunnable* runnable = new TCSoapRunnable();
        runnable->SetListenArguments(sConfigMgr->GetStringDefault("SOAP.IP", "127.0.0.1"), uint16(sConfigMgr->GetIntDefault("SOAP.Port", 7878)));
        soapThread = new ACE_Based::Thread(runnable);
    }

    ///- Start up freeze catcher thread
    if (uint32 freezeDelay = sConfigMgr->GetIntDefault("MaxCoreStuckTime", 0))
    {
        FreezeDetectorRunnable* fdr = new FreezeDetectorRunnable();
        fdr->SetDelayTime(freezeDelay * 1000);
        ACE_Based::Thread freezeThread(fdr);
        freezeThread.setPriority(ACE_Based::Highest);
    }

    ///- Launch the world listener socket
    uint16 worldPort = uint16(sWorld->getIntConfig(CONFIG_PORT_WORLD));
    std::string bindIp = sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");

    if (sWorldSocketMgr->StartNetwork(worldPort, bindIp.c_str()) == -1)
    {
        TC_LOG_ERROR("server.Mondial", "Impossible de démarrer le réseau");
        World::StopNow(ERROR_EXIT_CODE);
        // go down and shutdown the server
    }

    // set server online (allow connecting now)
    LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = flag & ~%u, population = 0 WHERE id = '%u'", REALM_FLAG_INVALID, realmID);

    TC_LOG_INFO("server.Mondial", "%s (Mondial-daemon) prêt...", _FULLVERSION);

    // when the main thread closes the singletons get unloaded
    // since worldrunnable uses them, it will crash if unloaded after master
    worldThread.wait();
    rarThread.wait();

    if (soapThread)
    {
        soapThread->wait();
        soapThread->destroy();
        delete soapThread;
    }

    // set server offline
    LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = flag | %u WHERE id = '%d'", REALM_FLAG_OFFLINE, realmID);

    ///- Clean database before leaving
    ClearOnlineAccounts();

    _StopDB();

    TC_LOG_INFO("server.Mondial", "Arrêt du processus...");

    if (cliThread)
    {
        #ifdef _WIN32

        // this only way to terminate CLI thread exist at Win32 (alt. way exist only in Windows Vista API)
        //_exit(1);
        // send keyboard input to safely unblock the CLI thread
        INPUT_RECORD b[4];
        HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
        b[0].EventType = KEY_EVENT;
        b[0].Event.KeyEvent.bKeyDown = TRUE;
        b[0].Event.KeyEvent.uChar.AsciiChar = 'X';
        b[0].Event.KeyEvent.wVirtualKeyCode = 'X';
        b[0].Event.KeyEvent.wRepeatCount = 1;

        b[1].EventType = KEY_EVENT;
        b[1].Event.KeyEvent.bKeyDown = FALSE;
        b[1].Event.KeyEvent.uChar.AsciiChar = 'X';
        b[1].Event.KeyEvent.wVirtualKeyCode = 'X';
        b[1].Event.KeyEvent.wRepeatCount = 1;

        b[2].EventType = KEY_EVENT;
        b[2].Event.KeyEvent.bKeyDown = TRUE;
        b[2].Event.KeyEvent.dwControlKeyState = 0;
        b[2].Event.KeyEvent.uChar.AsciiChar = '\r';
        b[2].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
        b[2].Event.KeyEvent.wRepeatCount = 1;
        b[2].Event.KeyEvent.wVirtualScanCode = 0x1c;

        b[3].EventType = KEY_EVENT;
        b[3].Event.KeyEvent.bKeyDown = FALSE;
        b[3].Event.KeyEvent.dwControlKeyState = 0;
        b[3].Event.KeyEvent.uChar.AsciiChar = '\r';
        b[3].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
        b[3].Event.KeyEvent.wVirtualScanCode = 0x1c;
        b[3].Event.KeyEvent.wRepeatCount = 1;
        DWORD numb;
        WriteConsoleInput(hStdIn, b, 4, &numb);

        cliThread->wait();

        #else

        cliThread->destroy();

        #endif

        delete cliThread;
    }

    // for some unknown reason, unloading scripts here and not in worldrunnable
    // fixes a memory leak related to detaching threads from the module
    //UnloadScriptingModule();

    OpenSSLCrypto::threadsCleanup();
    // Exit the process with specified return value
    return World::GetExitCode();
}

/// Initialize connection to the databases
bool Master::_StartDB()
{
    MySQL::Library_Init();

    std::string dbString;
    uint8 asyncThreads, synchThreads;

    dbString = sConfigMgr->GetStringDefault("WorldDatabaseInfo", "");
    if (dbString.empty())
    {
        TC_LOG_ERROR("server.Mondial", "Base de données mondiale n'est pas spécifié dans le fichier de configuration");
        return false;
    }

    asyncThreads = uint8(sConfigMgr->GetIntDefault("WorldDatabase.WorkerThreads", 1));
    if (asyncThreads < 1 || asyncThreads > 32)
    {
        TC_LOG_ERROR("server.Mondial", "Base de données mondiale: nombre invalide de fil spécifié. "
            "S'il vous plaît choisir une valeur comprise entre 1 et 32.");
        return false;
    }

    synchThreads = uint8(sConfigMgr->GetIntDefault("WorldDatabase.SynchThreads", 1));
    ///- Initialize the world database
    if (!WorldDatabase.Open(dbString, asyncThreads, synchThreads))
    {
        TC_LOG_ERROR("server.Mondial", "Impossible de se connecter à la base de données mondiale %s", dbString.c_str());
        return false;
    }

    ///- Get character database info from configuration file
    dbString = sConfigMgr->GetStringDefault("CharacterDatabaseInfo", "");
    if (dbString.empty())
    {
        TC_LOG_ERROR("server.Mondial", "Base de données de caractère non spécifié dans le fichier de configuration");
        return false;
    }

    asyncThreads = uint8(sConfigMgr->GetIntDefault("CharacterDatabase.WorkerThreads", 1));
    if (asyncThreads < 1 || asyncThreads > 32)
    {
        TC_LOG_ERROR("server.Mondial", "Base de données de caractère: nombre invalide de fil spécifié. "
            "S'il vous plaît choisir une valeur comprise entre 1 et 32.");
        return false;
    }

    synchThreads = uint8(sConfigMgr->GetIntDefault("CharacterDatabase.SynchThreads", 2));

    ///- Initialize the Character database
    if (!CharacterDatabase.Open(dbString, asyncThreads, synchThreads))
    {
        TC_LOG_ERROR("server.Mondial", "Impossible de se connecter à la base de données de caractère %s", dbString.c_str());
        return false;
    }

    ///- Get login database info from configuration file
    dbString = sConfigMgr->GetStringDefault("LoginDatabaseInfo", "");
    if (dbString.empty())
    {
        TC_LOG_ERROR("server.Mondial", "Base de données de connexion n'est pas spécifié dans le fichier de configuration");
        return false;
    }

    asyncThreads = uint8(sConfigMgr->GetIntDefault("LoginDatabase.WorkerThreads", 1));
    if (asyncThreads < 1 || asyncThreads > 32)
    {
        TC_LOG_ERROR("server.Mondial", "Base de données de connexion: nombre invalide de fil spécifié. "
            "S'il vous plaît choisir une valeur comprise entre 1 et 32.");
        return false;
    }

    synchThreads = uint8(sConfigMgr->GetIntDefault("LoginDatabase.SynchThreads", 1));
    ///- Initialise the login database
    if (!LoginDatabase.Open(dbString, asyncThreads, synchThreads))
    {
        TC_LOG_ERROR("server.Mondial", "Impossible de se connecter à la base de données de connexion %s", dbString.c_str());
        return false;
    }

    ///- Get the realm Id from the configuration file
    realmID = sConfigMgr->GetIntDefault("RealmID", 0);
    if (!realmID)
    {
        TC_LOG_ERROR("server.Mondial", "ID de Royaume n'est pas définie dans le fichier de configuration");
        return false;
    }

    // Load realm names into a store
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_REALMLIST);
    PreparedQueryResult result = LoginDatabase.Query(stmt);
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            realmNameStore[fields[0].GetUInt32()] = fields[1].GetString(); // Store the realm name into the store
        }
        while (result->NextRow());
    }

    TC_LOG_INFO("server.Mondial", "Royaume lancé avec l'ID de royaume %d", realmID);

    ///- Clean the database before starting
    ClearOnlineAccounts();

    ///- Insert version info into DB
    WorldDatabase.PExecute("UPDATE version SET core_version = '%s', core_revision = '%s'", _FULLVERSION, _HASH);        // One-time query

    sWorld->LoadDBVersion();

    TC_LOG_INFO("server.Mondial", "Utilisation de la DB mondiale: %s", sWorld->GetDBVersion());
    return true;
}

void Master::_StopDB()
{
    CharacterDatabase.Close();
    WorldDatabase.Close();
    LoginDatabase.Close();

    MySQL::Library_End();
}

/// Clear 'online' status for all accounts with characters in this realm
void Master::ClearOnlineAccounts()
{
    // Reset online status for all accounts with characters on the current realm
    LoginDatabase.DirectPExecute("UPDATE account SET online = 0 WHERE online > 0 AND id IN (SELECT acctid FROM realmcharacters WHERE realmid = %d)", realmID);

    // Reset online status for all characters
    CharacterDatabase.DirectExecute("UPDATE characters SET online = 0 WHERE online <> 0");

    // Battleground instance ids reset at server restart
    CharacterDatabase.DirectExecute("UPDATE character_battleground_data SET instanceId = 0");
}
