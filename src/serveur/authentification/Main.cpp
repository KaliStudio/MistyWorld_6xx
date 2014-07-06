/**
* @file main.cpp
* @brief Authentication Server main program
*
* This file contains the main program for the
* authentication serveur
*/

#include <ace/Dev_Poll_Reactor.h>
#include <ace/TP_Reactor.h>
#include <ace/ACE.h>
#include <ace/Sig_Handler.h>
#include <openssl/opensslv.h>
#include <openssl/crypto.h>

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Configuration/Config.h"
#include "Log.h"
#include "SystemConfig.h"
#include "Util.h"
#include "SignalHandler.h"
#include "RealmList.h"
#include "RealmAcceptor.h"

#ifdef __linux__
#include <sched.h>
#include <sys/resource.h>
#define PROCESS_HIGH_PRIORITY -15 // [-20, 19], default is 0
#endif

#ifndef _TRINITY_REALM_CONFIG
# define _TRINITY_REALM_CONFIG  "authentification.conf"
#endif

bool StartDB();
void StopDB();

bool stopEvent = false;                                     // Setting it to true stops the serveur

LoginDatabaseWorkerPool LoginDatabase;                      // Accessor to the authentification database

/// Handle authentification's termination signals
class AuthServerSignalHandler : public Trinity::SignalHandler
{
public:
    virtual void HandleSignal(int sigNum)
    {
        switch (sigNum)
        {
        case SIGINT:
        case SIGTERM:
            stopEvent = true;
            break;
        }
    }
};

/// Print out the usage string for this program on the console.
void usage(const char* prog)
{
    TC_LOG_INFO("serveur.authentification", "Utilisation: \n %s [<options>]\n"
        "    -c config_file           utiliser config_file comme fichier de configuration\n\r",
        prog);
}

/// Launch the auth serveur
extern int main(int argc, char** argv)
{
    // Command line parsing to get the configuration file name
    char const* configFile = _TRINITY_REALM_CONFIG;
    int count = 1;
    while (count < argc)
    {
        if (strcmp(argv[count], "-c") == 0)
        {
            if (++count >= argc)
            {
                printf("Runtime-Error: -c option nécessite un argument d'entrée\n");
                usage(argv[0]);
                return 1;
            }
            else
                configFile = argv[count];
        }
        ++count;
    }

    if (!sConfigMgr->LoadInitial(configFile))
    {
        printf("Fichier de configuration incorrect ou manquant : %s\n", configFile);
        printf("Vérifiez que le fichier existe et a \'[authentification]\' écrite dans la partie supérieure du dossier!\n");
        return 1;
    }

    TC_LOG_INFO("serveur.authentification", "%s (authentification)", _FULLVERSION);
    TC_LOG_INFO("serveur.authentification", "<Ctrl-C> pour stopper.\n");
    
	TC_LOG_INFO("serveur.authentification", " _____ _     _            _ _ _         _   _");
	TC_LOG_INFO("serveur.authentification", "|     |_|___| |_ _ _     | | | |___ _ _| |_| |");
	TC_LOG_INFO("serveur.authentification", "| | | | |_ -|  _| | |    | | | | . | '_| | . |");
	TC_LOG_INFO("serveur.authentification", "|_|_|_|_|___|_| |_  |    |_____|___|_| |_|___|");
	TC_LOG_INFO("serveur.authentification", "            |___|                         ");
	TC_LOG_INFO("serveur.authentification", "Project Misty World 2014(c) Emulateur WoW");
	TC_LOG_INFO("serveur.authentification", "<http://misty-world.site88.net/> \n");

    TC_LOG_INFO("serveur.authentification", "Utilisant le fichier de configuration %s.", configFile);

    TC_LOG_WARN("serveur.authentification", "%s (Librarie: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));

#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)
    ACE_Reactor::instance(new ACE_Reactor(new ACE_Dev_Poll_Reactor(ACE::max_handles(), 1), 1), true);
#else
    ACE_Reactor::instance(new ACE_Reactor(new ACE_TP_Reactor(), true), true);
#endif

    TC_LOG_DEBUG("serveur.authentification", "Le maximum de fichier ouvert est de %d", ACE::max_handles());

    // authentification PID file creation
    std::string pidFile = sConfigMgr->GetStringDefault("PidFile", "");
    if (!pidFile.empty())
    {
        if (uint32 pid = CreatePIDFile(pidFile))
            TC_LOG_INFO("serveur.authentification", "Daemon PID: %u\n", pid);
        else
        {
            TC_LOG_ERROR("serveur.authentification", "Vous ne pouvez pas de créer le fichier PID %s.\n", pidFile.c_str());
            return 1;
        }
    }

    // Initialize the database connection
    if (!StartDB())
        return 1;

    // Get the list of realms for the serveur
    sRealmList->Initialize(sConfigMgr->GetIntDefault("RealmsStateUpdateDelay", 20));
    if (sRealmList->size() == 0)
    {
        TC_LOG_ERROR("serveur.authentification", "Pas de royaumes valides spécifiés.");
        return 1;
    }

    // Launch the listening network socket
    RealmAcceptor acceptor;

    int32 rmport = sConfigMgr->GetIntDefault("RealmServerPort", 3724);
    if (rmport < 0 || rmport > 0xFFFF)
    {
        TC_LOG_ERROR("serveur.authentification", "Port spécifié sur la plage autorisée (1-65535)");
        return 1;
    }

    std::string bind_ip = sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");

    ACE_INET_Addr bind_addr(uint16(rmport), bind_ip.c_str());

    if (acceptor.open(bind_addr, ACE_Reactor::instance(), ACE_NONBLOCK) == -1)
    {
        TC_LOG_ERROR("serveur.authentification", "Le serveur d`authentification ne peut pas se lier à %s:%d", bind_ip.c_str(), rmport);
        return 1;
    }

    // Initialize the signal handlers
    AuthServerSignalHandler SignalINT, SignalTERM;

    // Register authentifications's signal handlers
    ACE_Sig_Handler Handler;
    Handler.register_handler(SIGINT, &SignalINT);
    Handler.register_handler(SIGTERM, &SignalTERM);

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
                    TC_LOG_ERROR("serveur.authentification", "Les processeurs marqués dans UseProcessors bitmask (hex) %x ne sont pas accessibles pour l'authentification. Processeurs accessibles bitmask (hex): %x", affinity, appAff);
                else if (SetProcessAffinityMask(hProcess, currentAffinity))
                    TC_LOG_INFO("serveur.authentification", "Utilisation des processeurs (bitmask, hex): %x", currentAffinity);
                else
                    TC_LOG_ERROR("serveur.authentification", "Impossible de définir les processeurs utilisés (hex): %x", currentAffinity);
            }
        }

        if (highPriority)
        {
            if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
                TC_LOG_INFO("serveur.authentification", "processus d'authentification niveau de priorité passe en niveau HAUT");
            else
                TC_LOG_ERROR("serveur.authentification", "Impossible de définir les le processus d'authentification niveau de priorité.");
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
            TC_LOG_ERROR("serveur.authentification", "Can't set used processors (hex): %x, error: %s", affinity, strerror(errno));
        else
        {
            CPU_ZERO(&mask);
            sched_getaffinity(0, sizeof(mask), &mask);
            TC_LOG_INFO("serveur.authentification", "Using processors (bitmask, hex): %x", *(uint32*)(&mask));
        }
    }

    if (highPriority)
    {
        if (setpriority(PRIO_PROCESS, 0, PROCESS_HIGH_PRIORITY))
            TC_LOG_ERROR("serveur.authentification", "Can't set authentification process priority class, error: %s", strerror(errno));
        else
            TC_LOG_INFO("serveur.authentification", "authentification process priority class set to %i", getpriority(PRIO_PROCESS, 0));
    }

#endif

    // maximum counter for next ping
    uint32 numLoops = (sConfigMgr->GetIntDefault("MaxPingTime", 30) * (MINUTE * 1000000 / 100000));
    uint32 loopCounter = 0;

    // Wait for termination signal
    while (!stopEvent)
    {
        // dont move this outside the loop, the reactor will modify it
        ACE_Time_Value interval(0, 100000);

        if (ACE_Reactor::instance()->run_reactor_event_loop(interval) == -1)
            break;

        if ((++loopCounter) == numLoops)
        {
            loopCounter = 0;
            TC_LOG_INFO("serveur.authentification", "Ping MySQL pour garder la connexion active");
            LoginDatabase.KeepAlive();
        }
    }

    // Close the Database Pool and library
    StopDB();

    TC_LOG_INFO("serveur.authentification", "Arrêt du processus...");
    return 0;
}

/// Initialize connection to the database
bool StartDB()
{
    MySQL::Library_Init();

    std::string dbstring = sConfigMgr->GetStringDefault("LoginDatabaseInfo", "");
    if (dbstring.empty())
    {
        TC_LOG_ERROR("serveur.authentification", "Base de données non spécifiée");
        return false;
    }

    int32 worker_threads = sConfigMgr->GetIntDefault("LoginDatabase.WorkerThreads", 1);
    if (worker_threads < 1 || worker_threads > 32)
    {
        TC_LOG_ERROR("serveur.authentification", "Valeur incorrecte spécifiée pour la base de données de connexion.Fils de travail, par défaut à 1.");
        worker_threads = 1;
    }

    int32 synch_threads = sConfigMgr->GetIntDefault("LoginDatabase.SynchThreads", 1);
    if (synch_threads < 1 || synch_threads > 32)
    {
        TC_LOG_ERROR("serveur.authentification", "Valeur incorrecte spécifiée pour la base de données de connexion. Synchronisation du fils, par défaut à 1.");
        synch_threads = 1;
    }
 
    // NOTE: While authentification is singlethreaded you should keep synch_threads == 1. Increasing it is just silly since only 1 will be used ever.
    if (!LoginDatabase.Open(dbstring, uint8(worker_threads), uint8(synch_threads)))
    {
        TC_LOG_ERROR("serveur.authentification", "Impossible de se connecter à la base de données");
        return false;
    }

    TC_LOG_INFO("serveur.authentification", "Regroupement de connexion de base de données d'authentification commencé.");
    sLog->SetRealmId(0); // Enables DB appenders when realm is set.
    return true;
}

/// Close the connection to the database
void StopDB()
{
    LoginDatabase.Close();
    MySQL::Library_End();
}
