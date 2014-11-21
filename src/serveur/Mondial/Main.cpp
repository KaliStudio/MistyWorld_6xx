

/// \addtogroup Trinityd TRINITY Daemon
/// @{
/// \file

#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <ace/Version.h>

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Configuration/Config.h"

#include "Log.h"
#include "Master.h"
#include "World.h"

#ifndef _TRINITY_CORE_CONFIG
# define _TRINITY_CORE_CONFIG  "Mondial.conf"
#endif

#ifdef _WIN32
#include "ServiceWin32.h"
char serviceName[] = "Mondial";
char serviceLongName[] = "service Misty World";
char serviceDescription[] = "Emulateur World of Warcraft world service Misty World";
/*
 * -1 - not in service mode
 *  0 - stopped
 *  1 - running
 *  2 - paused
 */
int m_ServiceStatus = -1;
#endif

WorldDatabaseWorkerPool WorldDatabase;                      ///< Accessor to the world database
CharacterDatabaseWorkerPool CharacterDatabase;              ///< Accessor to the character database
LoginDatabaseWorkerPool LoginDatabase;                      ///< Accessor to the realm/login database

RealmNameMap realmNameStore;
uint32 realmID;                                             ///< Id of the realm

/// Print out the usage string for this program on the console.
void usage(const char* prog)
{
    printf("Utilisation:\n");
    printf(" %s [<options>]\n", prog);
    printf("    -c config_file           utiliser config_file comme fichier de configuration\n");
#ifdef _WIN32
    printf("    Running as service functions:\n");
    printf("    --service                gérer comme service\n");
    printf("    -s install               installer le service\n");
    printf("    -s uninstall             désinstaller le service\n");
#endif
}

/// Launch the TRINITY server
extern int main(int argc, char** argv)
{
    ///- Command line parsing to get the configuration file name
    char const* cfg_file = _TRINITY_CORE_CONFIG;
    int c = 1;
    while (c < argc)
    {
        if (!strcmp(argv[c], "-c"))
        {
            if (++c >= argc)
            {
                printf("Runtime-Error: -c option nécessite un argument d'entrée");
                usage(argv[0]);
                return 1;
            }
            else
                cfg_file = argv[c];
        }

        #ifdef _WIN32
        if (strcmp(argv[c], "-s") == 0) // Services
        {
            if (++c >= argc)
            {
                printf("Runtime-Error: -s option nécessite un argument d'entrée");
                usage(argv[0]);
                return 1;
            }

            if (strcmp(argv[c], "install") == 0)
            {
                if (WinServiceInstall())
                    printf("Installation du service\n");
                return 1;
            }
            else if (strcmp(argv[c], "uninstall") == 0)
            {
                if (WinServiceUninstall())
                    printf("Désinstallation du service\n");
                return 1;
            }
            else
            {
                printf("Runtime-Error: Option non prise en charge %s", argv[c]);
                usage(argv[0]);
                return 1;
            }
        }

        if (strcmp(argv[c], "--service") == 0)
            WinServiceRun();
        #endif
        ++c;
    }

    if (!sConfigMgr->LoadInitial(cfg_file))
    {
        printf("Fichier de configuration incorrect ou manquant : %s\n", cfg_file);
        printf("Vérifiez que le fichier existe et est \'[Mondial]' écrite dans la partie supérieure du dossier!\n");
        return 1;
    }

    TC_LOG_INFO("server.Mondial", "Utilisant le fichier de configuration %s.", cfg_file);

    TC_LOG_INFO("server.Mondial", "Utilisant la version SSL: %s (librarie: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));
    TC_LOG_INFO("server.Mondial", "Utilisant la version ACE: %s", ACE_VERSION);

    ///- and run the 'Master'
    /// @todo Why do we need this 'Master'? Can't all of this be in the Main as for Realmd?
    int ret = sMaster->Run();

    // at sMaster return function exist with codes
    // 0 - normal shutdown
    // 1 - shutdown at error
    // 2 - restart command used, this code can be used by restarter for restart Trinityd

    return ret;
}

/// @}
