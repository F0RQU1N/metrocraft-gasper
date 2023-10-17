#include "cheat.h"
#include "../gasper.h"
#include "../../sdk/minecraft/player/player.h"
#include "../../sdk/minecraft/world/world.h"

cheats::c_cheats::~c_cheats( )
{
	_functions.clear( );
}

void cheats::c_cheats::register_function( std::function<void( std::shared_ptr<c_context> )> fn )
{
	_functions.push_back( fn );
}

void cheats::c_cheats::invoke( std::shared_ptr<c_context> ctx )
{
	const auto local_obj = ctx->local->get_object( );
	if ( local_obj ) {
		if ( gasper::instance->b_fullbright ) {
			static jclass cPotionEffect = gasper::instance->find_class( "net.minecraft.potion.PotionEffect" );
			jobject PotionEffectObj = gasper::instance->get_env( )->AllocObject( cPotionEffect );

			gasper::instance->SetField( PotionEffectObj, "field_76462_a", "I", 16 ); // potionID
			gasper::instance->SetField( PotionEffectObj, "field_76460_b", "I", 1337 ); // duration
			gasper::instance->SetField( PotionEffectObj, "field_76461_c", "I", 255 ); // amplifier
			gasper::instance->SetField( PotionEffectObj, "field_82723_d", "Z", false ); // isSplashPotion
			gasper::instance->SetField( PotionEffectObj, "field_82724_e", "Z", false ); // isAmbient
			gasper::instance->SetField( PotionEffectObj, "field_100013_f", "Z", true ); // isPotionDurationMax

			gasper::instance->CallMethod<void>( local_obj, "func_70690_d", "(Lnet/minecraft/potion/PotionEffect;)V", PotionEffectObj );

			gasper::instance->get_env( )->DeleteLocalRef( PotionEffectObj );
		}
		if ( gasper::instance->b_speedhack ) {
			if ( ctx->local->isOnGround( ) && !ctx->local->isInWater( ) ) {
				float dir = ctx->local->getRotationYaw( );
				if ( ctx->local->getMoveForward( ) < 0.0F ) {
					dir += 180.0F;
				}
				if ( ctx->local->getMoveStrafing( ) > 0.0F ) {
					dir -= 90.0F * ( ctx->local->getMoveForward( ) > 0.0F ? 0.68F
							: ctx->local->getMoveForward( ) < 0.0F ? -0.5F : 1.0F );
				}
				if ( ctx->local->getMoveStrafing( ) < 0.0F ) {
					dir += 90.0F * ( ctx->local->getMoveForward( ) > 0.0F ? 0.68F
							: ctx->local->getMoveForward( ) < 0.0F ? -0.5F : 1.0F );
				}
				double hOff = 0.221;
				if ( ctx->local->isSprinting( ) ) {
					hOff *= 0.3190000119209289;
				}
				if ( ctx->local->isSneaking( ) ) {
					hOff *= 0.3;
				}
				hOff = gasper::instance->f_shmp;
				float var9 = (float)( (float)std::cos( ( dir + 90.0F ) * 3.141592653589793 / 180.0 )
						* hOff );
				float zD = (float)( (float)std::sin( ( dir + 90.0F ) * 3.141592653589793 / 180.0 )
						* hOff );
				if ( ctx->local->getMoveForward( ) != 0 || ctx->local->getMoveStrafing( ) != 0 )
				{
					ctx->local->setMotionX( var9 );
					ctx->local->setMotionZ( zD );
				}
			}
		}

		if ( GetAsyncKeyState( VK_UP ) & 1 ) {
			auto pos = ctx->local->get_position( );
			pos.y += 6;
			ctx->local->set_position( pos );
		}
		else if ( GetAsyncKeyState( VK_DOWN ) & 1 ) {
			auto pos = ctx->local->get_position( );
			pos.y -= 6;
			ctx->local->set_position( pos );
		}
	}

	for ( auto& fn : _functions )
		fn( ctx );
}