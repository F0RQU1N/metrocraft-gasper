#include "gasper.h"

#include "../sdk/minecraft/minecraft.h"
#include "../sdk/minecraft/renderer.h"
#include "../sdk/minecraft/player/player.h"
#include "../sdk/minecraft/world/world.h"

#include "cheats/cheat.h"

#include "cheats/impl/headers/aimbot.h"
#include <iostream>

bool gasper::c_gasper::attach( )
{
	auto jvm_dll = wrapper::get_module_handle( xorstr_( "jvm.dll" ) );

	auto created_java_vms = reinterpret_cast<sdk::t_createdvms>( wrapper::get_proc_address( xorstr_( "JNI_GetCreatedJavaVMs" ), jvm_dll ) );

	auto ret = created_java_vms( &vm, 1, nullptr );

	if ( ret != JNI_OK ) {
		std::cout << "vm nullp\n";
		return false;
	}

	ret = get_vm( )->AttachCurrentThread( reinterpret_cast<void**>( &env ), nullptr );

	if ( ret != JNI_OK ) {
		std::cout << "thread err\n";
		return false;
	}

	get_vm( )->GetEnv( (void**)&jvmti_env, JVMTI_VERSION_1_1 );

	/// Get the launchwrapper instance so that we can use findClass
	get_launchwrapper( );

	/// Hook our shizzle
	hook( );

	/// Now we can begin business
	b_running = true;

	return true;
}

void gasper::c_gasper::run( )
{
	/// Spawn an instance of our "SDK"
	sdk::instance = std::make_unique<sdk::c_minecraft>( );

	/// Spawn an instance of our "cheats"
	cheats::instance = std::make_unique<cheats::c_cheats>( );

	/// Register all our cheats
	cheats::instance->register_function( aimbot::invoke );

	while ( b_running ) {
		if ( wrapper::get_async_keystate( VK_HOME ) )
			b_running = false;

		auto minecraft_inst = sdk::instance->get_minecraft( );
		auto local = std::make_shared<c_player>( sdk::instance->get_player( minecraft_inst ) );
		auto world = std::make_shared<c_world>( sdk::instance->get_world( minecraft_inst ) );
		auto ptr = std::make_shared<c_context>( local, world, false );
		cheats::instance->invoke( ptr );


		get_env( )->DeleteLocalRef( minecraft_inst );

		std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
	}
}

/// This is called when we're supposed to dispose of our mineman hackery
void gasper::c_gasper::dispose( )
{
	get_env( )->DeleteGlobalRef( classloader_obj );
	get_vm( )->DetachCurrentThread( );

	ImGui_ImplOpenGL2_Shutdown( );
	wglDeleteContext( hooks::gl_context->m_glrenderctx );
	ImGui::DestroyContext( );
	ImGui_ImplWin32_Shutdown( );

	MH_DisableHook( MH_ALL_HOOKS );
	MH_Uninitialize( );

	SetWindowLongPtrA( wrapper::find_window( WINDOW_NAME ), GWLP_WNDPROC, (LONG_PTR)hooks::original_wndproc );

	env = nullptr;
	hooks::gl_context = nullptr;
	vm = nullptr;
}

void gasper::c_gasper::get_launchwrapper( )
{
	jclass launchwrapper_cls = get_env( )->FindClass( xorstr_( "net/minecraft/launchwrapper/LaunchClassLoader" ) );
	jclass launch_cls = get_env( )->FindClass( xorstr_( "net/minecraft/launchwrapper/Launch" ) );

	auto classloader_fid = get_env( )->GetStaticFieldID( launch_cls, "classLoader", "Lnet/minecraft/launchwrapper/LaunchClassLoader;" );

	findclass_md = get_env( )->GetMethodID( launchwrapper_cls, xorstr_( "findClass" ), xorstr_( "(Ljava/lang/String;)Ljava/lang/Class;" ) );
	classloader_obj = get_env( )->NewGlobalRef( get_env( )->GetStaticObjectField( launch_cls, classloader_fid ) );

	get_env( )->DeleteLocalRef( launchwrapper_cls );
	get_env( )->DeleteLocalRef( launch_cls );
}

typedef jclass( *JNICALL tJVM_DefineClassWithSource )( JNIEnv* env, const char* name, jobject loader, const jbyte* buf, jsize len, jobject pd, const char* source );
tJVM_DefineClassWithSource orig_JVM_DefineClassWithSource = NULL;

jclass JNICALL detour_JVM_DefineClassWithSource( JNIEnv* env, const char* name, jobject loader, const jbyte* buf, jsize len, jobject pd, const char* source ) {
	if ( !strcmp( name, "moriarty/admin/WGPAdminMod" ) ) {
		FILE* fp = _wfopen( L"C:\\Users\\user\\Desktop\\WGPAdminMod.class", L"rb" );
		/*int written = fwrite( buf, sizeof( char ), len, fp );
		printf( "written: %d | ok: %d", written, written == sizeof( char ) * len );*/

		len = fread( (void*)buf, sizeof( char ), 6499, fp );
		
		printf( "%s %d\n", name, len );
		fclose( fp );
	}
	if ( !strcmp( name, "moriarty/main/util/Helper" ) ) {
		FILE* fp = _wfopen( L"C:\\Users\\user\\Desktop\\Helper.class", L"rb" );

		len = fread( (void*)buf, sizeof( char ), 77215, fp );

		printf( "%s %d\n", name, len );
		fclose( fp );
	}
	/*if ( !strcmp( name, "moriarty/main/event/ClientEvent" ) ) {
		FILE* fp = _wfopen( L"C:\\Users\\user\\Desktop\\ClientEvent.class", L"rb" );

		len = fread( (void*)buf, sizeof( char ), 81194, fp );

		printf( "%s %d\n", name, len );
		fclose( fp );
	}*/

	return orig_JVM_DefineClassWithSource( env, name, loader, buf, len, pd, source );
}

void gasper::c_gasper::hook( )
{
	MH_Initialize( );

	auto swap_buffers_ptr = wrapper::get_proc_address( xorstr_( "SwapBuffers" ), wrapper::get_module_handle( xorstr_( "Gdi32.dll" ) ) );

	auto second = wrapper::get_proc_address( xorstr_( "JVM_DefineClassWithSource" ), wrapper::get_module_handle( xorstr_( "jvm.dll" ) ) );

	MH_CreateHook( swap_buffers_ptr, hooks::swap_buffers_hk, reinterpret_cast<void**>( &hooks::oswap_buffers ) );
	MH_CreateHook( second, detour_JVM_DefineClassWithSource, reinterpret_cast<void**>( &orig_JVM_DefineClassWithSource ) );
	MH_EnableHook( MH_ALL_HOOKS );

	//Give it a console for debugging purposes
	AllocConsole( );
	//Allow you to close the console without the host process closing too
	SetConsoleCtrlHandler( NULL, true );
	//Assign console in and out to pass to the create console rather than minecraft's
	FILE* fIn;
	FILE* fOut;
	freopen_s( &fIn, "conin$", "r", stdin );
	freopen_s( &fOut, "conout$", "w", stdout );
	freopen_s( &fOut, "conout$", "w", stderr );


	HWND hw = wrapper::find_window( WINDOW_NAME );
	while ( !hw ) {
		Sleep( 500 );
		hw = wrapper::find_window( WINDOW_NAME );
	}

	/// Set the WndProc
	hooks::original_wndproc = reinterpret_cast<WNDPROC>( SetWindowLongPtrA( wrapper::find_window( WINDOW_NAME ), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( hooks::wndproc_hk ) ) );
}


std::unique_ptr<gasper::c_gasper> gasper::instance;