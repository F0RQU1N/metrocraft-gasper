#include "../headers/aimbot.h"

void aimbot::invoke( std::shared_ptr<c_context> ctx ) {
	const auto is_sane = [&]( ) {
		return ( ctx->local->get_object( ) && ctx->world->get_object( ) && ctx->local->get_health( ) > 0 );
	};

	/// Get closest player to local player (distance wise)
	const auto get_closest_player = [&]( ) {
		/// If you still use some random number you're stupid
		double dist = ( std::numeric_limits<double>::max )( );
		/// Target :O
		std::shared_ptr<c_player> target = nullptr;

		for ( const auto& player : ctx->world->get_players( ) ) {
			if ( gasper::instance->get_env( )->IsSameObject( ctx->local->get_object( ), player->get_object( ) ) )
				continue;

			if ( player->get_health( ) <= 0 )
				continue;

			if ( player->is_invisible( ) )
				continue;

			if ( !ctx->local->can_entity_be_seen( player->get_object( ) ) )
				continue;

			/// Basic stuff
			if ( ctx->local->get_distance_to( player ) <= dist )
			{
				dist = ctx->local->get_distance_to( player );
				target = player;
			}
		}

		return target;
	};

	if ( !is_sane( ) || !aimbot::m_enabled || ctx->local->get_health( ) <= 0 )
		return;

	if ( auto target = get_closest_player( ); target ) {
		auto angles = sdk::util::get_angles( ctx->local->get_position( ), target->get_position( ), target->get_height( ) );

		auto difference = sdk::util::wrap_to_180( -( ctx->local->getRotationYaw( ) - angles.first ) );
		auto difference2 = sdk::util::wrap_to_180( -( ctx->local->get_pitch( ) - angles.second ) );

		aimbot::target_info = std::format( "target: {} [{}]", target->get_display_name( ), target->get_health( ) );

		auto current_yaw = ctx->local->getRotationYaw( );
		current_yaw += ( difference );

		auto current_pitch = ctx->local->get_pitch( );
		current_pitch += ( difference2 );

		ctx->local->set_yaw( current_yaw );
		ctx->local->set_pitch( current_pitch );

	}
	else {
		aimbot::target_info.clear( );
	}
}