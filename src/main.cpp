#include <stdio.h>

#include "cbase.h"
#include "interface.h"
#include "filesystem.h"
#include "engine/iserverplugin.h"
#include "eiface.h"
#include "igameevents.h"
#include "convar.h"
#include "Color.h"
#include "vstdlib/random.h"
#include "engine/IEngineTrace.h"
#include "tier2/tier2.h"
#include "tier1/utlbuffer.h"

#include "luacontext.h"
#include "interfaces.h"
#include "luaReference.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//---------------------------------------------------------------------------------
// Purpose: a sample 3rd party plugin class
//---------------------------------------------------------------------------------
class CLuaPlugin: public IServerPluginCallbacks
{
public:
    CLuaPlugin();
    ~CLuaPlugin();

    // IServerPluginCallbacks methods
    virtual bool            Load(   CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory );
    virtual void            Unload( void );
    virtual void            Pause( void );
    virtual void            UnPause( void );
    virtual const char     *GetPluginDescription( void );      
    virtual void            LevelInit( char const *pMapName );
    virtual void            ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
    virtual void            GameFrame( bool simulating );
    virtual void            LevelShutdown( void );
    virtual void            ClientActive( edict_t *pEntity );
    virtual void            ClientDisconnect( edict_t *pEntity );
    virtual void            ClientPutInServer( edict_t *pEntity, char const *playername );
    virtual void            SetCommandClient( int index );
    virtual void            ClientSettingsChanged( edict_t *pEdict );
    virtual PLUGIN_RESULT   ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen );
    virtual PLUGIN_RESULT   ClientCommand( edict_t *pEntity, const CCommand &args );
    virtual PLUGIN_RESULT   NetworkIDValidated( const char *pszUserName, const char *pszNetworkID );
    virtual void            OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue );
    virtual void            OnEdictAllocated( edict_t *edict );
    virtual void            OnEdictFreed( const edict_t *edict  );
};


// 
// The plugin is a static singleton that is exported as an interface
//
CLuaPlugin g_LuaPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_LuaPlugin );

CLuaPlugin::CLuaPlugin()
{
}

CLuaPlugin::~CLuaPlugin()
{
}

CON_COMMAND( lua_runstring, "run a string of lua" )
{
    if ( args.ArgC() != 2 )
    {
        Msg ( "lua_runstring <lua>\n" );
        return;
    }
    const char *lua = args.Arg( 1 );
    int error = luaL_dostring(g_pLuaContext->state, lua);
    if (error) {
      Warning("%s\n", lua_tostring(g_pLuaContext->state, -1));
      lua_pop(g_pLuaContext->state, 1);  /* pop error message from the stack */
    }
}

CON_COMMAND( lua_run, "run <filename>.lua in lua folder" )
{
    if ( args.ArgC() != 2 )
    {
        Msg ( "lua_run <filename>\n" );
        return;
    }
    const char *value = args.Arg( 1 );
    char filename[ 512 ];
    Q_snprintf( filename, sizeof( filename ), "%s.lua", value );

    if (!g_pFullFileSystem->FileExists(filename, "LUA")) {
        Msg("File not found\n");
        return;
    }

    CUtlBuffer fileBuffer(0, 0, CUtlBuffer::READ_ONLY | CUtlBuffer::TEXT_BUFFER );
    if (!g_pFullFileSystem->ReadFile(filename, "LUA", fileBuffer)) {
        Msg("Unable to load file\n");
        return;
    }
    if (luaL_loadbuffer(g_pLuaContext->state, fileBuffer.String(), fileBuffer.TellPut(), filename) 
        || lua_pcall(g_pLuaContext->state, 0, LUA_MULTRET, 0)) {
      Warning("%s\n", lua_tostring(g_pLuaContext->state, -1));
      lua_pop(g_pLuaContext->state, 1);  /* pop error message from the stack */
    }
}


CON_COMMAND( lua_debugcallback, "call callback called 'test' with value" )
{
    if ( args.ArgC() < 2 )
    {
        Msg ( "lua_debugcallback <value>\n" );
        return;
    }
    const char *value = args.Arg( 1 );
    lua_pushstring(g_pLuaContext->state, value);
    g_pLuaContext->callHook("test", 1);

}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is loaded, load the interface we need from the engine
//---------------------------------------------------------------------------------
bool CLuaPlugin::Load(  CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory )
{
    ConnectTier1Libraries( &interfaceFactory, 1 );
    ConnectTier2Libraries( &interfaceFactory, 1 );
    ConVar_Register( 0 );
    FindInterfaces(interfaceFactory, gameServerFactory);
    if (!g_pFullFileSystem) {
        return false; // TODO: check more interfaces?
    }
    // TODO: handle this "jbmod" part dynamically
    // engine->GetGameDir ?
    g_pFullFileSystem->AddSearchPath( "jbmod/lua", "LUA" );
    g_pFullFileSystem->MarkPathIDByRequestOnly( "LUA", true );
    // this is probably bad code.
    g_pLuaContext = new luaContext();
    return true;
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//---------------------------------------------------------------------------------
void CLuaPlugin::Unload( void )
{
    delete g_pLuaContext;

    g_pFullFileSystem->RemoveSearchPaths( "LUA" );
    ConVar_Unregister( );
    DisconnectTier2Libraries( );
    DisconnectTier1Libraries( );
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is paused (i.e should stop running but isn't unloaded)
//---------------------------------------------------------------------------------
void CLuaPlugin::Pause( void )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unpaused (i.e should start executing again)
//---------------------------------------------------------------------------------
void CLuaPlugin::UnPause( void )
{
}

//---------------------------------------------------------------------------------
// Purpose: the name of this plugin, returned in "plugin_print" command
//---------------------------------------------------------------------------------
const char *CLuaPlugin::GetPluginDescription( void )
{
    return "lua mod by some guy named acuifex";
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CLuaPlugin::LevelInit( char const *pMapName )
{
    appendToStack(g_pLuaContext->state, pMapName);
    g_pLuaContext->callHook("LevelInit", 1);
}

//---------------------------------------------------------------------------------
// Purpose: called on level start, when the server is ready to accept client connections
//      edictCount is the number of entities in the level, clientMax is the max client count
//---------------------------------------------------------------------------------
void CLuaPlugin::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
}

//---------------------------------------------------------------------------------
// Purpose: called once per server frame, do recurring work here (like checking for timeouts)
//---------------------------------------------------------------------------------
void CLuaPlugin::GameFrame( bool simulating )
{
    appendToStack(g_pLuaContext->state, simulating);
    g_pLuaContext->callHook("GameFrame", 1);
}

//---------------------------------------------------------------------------------
// Purpose: called on level end (as the server is shutting down or going to a new map)
//---------------------------------------------------------------------------------
void CLuaPlugin::LevelShutdown( void ) // !!!!this can get called multiple times per map change
{
    g_pLuaContext->callHook("LevelShutdown", 0);
}

//---------------------------------------------------------------------------------
// Purpose: called when a client spawns into a server (i.e as they begin to play)
//---------------------------------------------------------------------------------
void CLuaPlugin::ClientActive( edict_t *pEntity )
{
    g_pLuaContext->callHook("ClientActive", 0);
}

//---------------------------------------------------------------------------------
// Purpose: called when a client leaves a server (or is timed out)
//---------------------------------------------------------------------------------
void CLuaPlugin::ClientDisconnect( edict_t *pEntity )
{
    g_pLuaContext->callHook("ClientDisconnect", 0);
}

//---------------------------------------------------------------------------------
// Purpose: called on 
//---------------------------------------------------------------------------------
void CLuaPlugin::ClientPutInServer( edict_t *pEntity, char const *playername )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CLuaPlugin::SetCommandClient( int index )
{
}
//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CLuaPlugin::ClientSettingsChanged( edict_t *pEdict )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when a client joins a server
//---------------------------------------------------------------------------------
PLUGIN_RESULT CLuaPlugin::ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
    return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a client types in a command (only a subset of commands however, not CON_COMMAND's)
//---------------------------------------------------------------------------------
PLUGIN_RESULT CLuaPlugin::ClientCommand( edict_t *pEntity, const CCommand &args )
{
    const char *pcmd = args[0];

    if ( !pEntity || pEntity->IsFree() ) 
    {
        return PLUGIN_CONTINUE;
    }
    // TODO: should have an option to stop execution
    appendToStack(g_pLuaContext->state, pcmd);
    g_pLuaContext->callHook("ClientCommand", 1);
    return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a client is authenticated
//---------------------------------------------------------------------------------
PLUGIN_RESULT CLuaPlugin::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
    return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a cvar value query is finished
//---------------------------------------------------------------------------------
void CLuaPlugin::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
{}
void CLuaPlugin::OnEdictAllocated( edict_t *edict )
{
}
void CLuaPlugin::OnEdictFreed( const edict_t *edict  )
{
}

//---------------------------------------------------------------------------------
// Purpose: an example cvar
//---------------------------------------------------------------------------------
