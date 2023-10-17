#include "../gasper.h"

#include "../cheats/impl/headers/aimbot.h"

/// Context
std::shared_ptr<wrapper::c_context> gasper::hooks::gl_context = nullptr;

/// Flag for initializing ImGui
std::once_flag flag;

int __stdcall gasper::hooks::swap_buffers_hk( HDC hdc ) {

	/// Le retardation has arrived
	static auto mc_window = wrapper::find_window( WINDOW_NAME );
	auto window_rect = wrapper::get_window_rect( WINDOW_NAME );

	/// std::call_once because we're hip
	std::call_once( flag, [&]( ) {
		gl_context = wrapper::create_gl_context( );
		ImGui::CreateContext( );
		ImGui_ImplWin32_Init( mc_window );
		ImGui_ImplOpenGL2_Init( );
		return gasper::hooks::oswap_buffers( hdc );
	} );

	/// if for some reason our instance is nullptr'd or we aren't running anymore but this hook still is
	/// make sure to dip
	if ( !gasper::instance || !gasper::instance->b_running )
		return gasper::hooks::oswap_buffers( hdc );

	wglMakeCurrent( gl_context->m_hdc_devicectx, gl_context->m_glrenderctx );

	ImGui_ImplOpenGL2_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	if ( gasper::instance->b_open ) {
		ImGuiIO& io = ImGui::GetIO( );
		io.MouseDrawCursor = true;

		ImGui::Begin( xorstr_( "metrohack" ), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize );
		ImGui::Checkbox( "Aim", &aimbot::m_enabled );
		ImGui::Checkbox( "Fullbright", &gasper::instance->b_fullbright );
		ImGui::Checkbox( "SpeedHack", &gasper::instance->b_speedhack );
		ImGui::SliderFloat( "Multiplier", &gasper::instance->f_shmp, 0.1f, 1.f );
		ImGui::Separator( );
		if ( ImGui::Button( "Unload" ) ) {
			FreeLibraryAndExitThread( gasper::instance->_module, 0 );
			MH_DisableHook( MH_ALL_HOOKS );
			MH_RemoveHook( MH_ALL_HOOKS );
		}
		

		ImGui::End( );
	}

	ImGuiIO& io = ImGui::GetIO( );
	io.MouseDrawCursor = false;

	if ( aimbot::m_enabled ) {
		auto drawlist = ImGui::GetBackgroundDrawList( );

		ImVec2 pos{ io.DisplaySize.x / 2, io.DisplaySize.y / 2 };

		drawlist->AddText( pos, 0xFFFFFFFF, aimbot::target_info.empty( ) ? "no target" : aimbot::target_info.c_str( ) );
	}
	/*auto& ctx = gasper::instance->context;
	if ( ctx && ctx->world->get_object( ) && ctx->local->get_object( ) && ctx->local->get_health( ) > 0 ) {
		for ( const auto& player : ctx->world->get_players( ) ) {
			if ( gasper::instance->get_env( )->IsSameObject( ctx->local->get_object( ), player->get_object( ) ) )
				continue;

			if ( player->get_health( ) <= 0 )
				continue;

			if ( player->is_invisible( ) )
				continue;

			drawlist->AddText( pos, 0xFFFFFFFF, aimbot::target_info.c_str( ) );
		}
	}*/

	ImGui::Render( );
	glViewport( 0, 0, window_rect.right, window_rect.top );
	ImGui_ImplOpenGL2_RenderDrawData( ImGui::GetDrawData( ) );

	wglMakeCurrent( gl_context->m_hdc_devicectx, gl_context->m_oglrenderctx );

	return gasper::hooks::oswap_buffers( hdc );
}