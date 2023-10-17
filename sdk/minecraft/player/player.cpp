#include "player.h"

#include "../../../main/gasper.h"

c_player::c_player( jobject obj )
{
	player_obj = obj;
}

c_player::~c_player( )
{
	gasper::instance->get_env( )->DeleteLocalRef( player_obj );
}

bool c_player::is_invisible( )
{
	jmethodID invisible_md = gasper::instance->get_env( )->GetMethodID( gasper::instance->get_env( )->GetObjectClass( player_obj ), MINECRAFT_1_8 ? xorstr_( "ax" ) : xorstr_( "func_82150_aj" ), xorstr_( "()Z" ) );

	return gasper::instance->get_env( )->CallBooleanMethod( player_obj, invisible_md );
}


void c_player::set_position( sdk::vec3d vec ) {
	gasper::instance->CallMethod<void>( player_obj, "func_70107_b", "(DDD)V", vec.x, vec.y, vec.z );
}

void c_player::setSprinting( bool state ) {
	gasper::instance->CallMethod<void>( player_obj, "func_70031_b", "(Z)V", state );
}

jobject c_player::get_capabilities( ) {
	auto e = gasper::instance->get_env( );
	jobject field_obj = gasper::instance->find_field( e->GetObjectClass( player_obj ), "field_71075_bZ" );
	auto fieldid = e->FromReflectedField( field_obj );
	return e->GetObjectField( player_obj, fieldid );
}

float c_player::getHurtTime( )
{
	jfieldID hurttime_fid = gasper::instance->get_env( )->GetFieldID( gasper::instance->get_env( )->GetObjectClass( player_obj ), MINECRAFT_1_8 ? xorstr_( "au" ) : xorstr_( "field_70737_aN" ), xorstr_( "F" ) );

	return gasper::instance->get_env( )->GetFloatField( player_obj, hurttime_fid );
}

float c_player::getMoveForward( ) {
	jfieldID hurttime_fid = gasper::instance->get_env( )->GetFieldID( gasper::instance->get_env( )->GetObjectClass( player_obj ), MINECRAFT_1_8 ? xorstr_( "ba" ) : xorstr_( "field_70701_bs" ), xorstr_( "F" ) );

	return gasper::instance->get_env( )->GetFloatField( player_obj, hurttime_fid );
}

void c_player::setMoveForward( float s ) {
	gasper::instance->SetField<float>( player_obj, "field_70701_bs", "F", s );
}

float c_player::getMoveStrafing( )
{
	jfieldID hurttime_fid = gasper::instance->get_env( )->GetFieldID( gasper::instance->get_env( )->GetObjectClass( player_obj ), "field_70702_br", "F" );

	return gasper::instance->get_env( )->GetFloatField( player_obj, hurttime_fid );
}

sdk::vec3d c_player::get_lasttick_position( ) {
	auto lastx_fid = gasper::instance->get_env( )->GetFieldID( gasper::instance->get_env( )->GetObjectClass( player_obj ), "field_70142_S", "D" );
	auto lasty_fid = gasper::instance->get_env( )->GetFieldID( gasper::instance->get_env( )->GetObjectClass( player_obj ), "field_70137_T", "D" );
	auto lastz_fid = gasper::instance->get_env( )->GetFieldID( gasper::instance->get_env( )->GetObjectClass( player_obj ), "field_70136_U", "D" );

	return sdk::vec3d{
		gasper::instance->get_env( )->GetDoubleField( player_obj, lastx_fid ),
			gasper::instance->get_env( )->GetDoubleField( player_obj, lasty_fid ),
			gasper::instance->get_env( )->GetDoubleField( player_obj, lastz_fid )
	};
}

SAxisAlignedBB c_player::get_native_bbox( ) {
	auto env = gasper::instance->get_env( );
	auto player_class = env->GetObjectClass( player_obj );
	jfieldID boundingbox_fid;

	boundingbox_fid = env->GetFieldID( player_class, "field_70121_D", "Lnet/minecraft/util/AxisAlignedBB;" );
	env->DeleteLocalRef( player_class );
	auto axisaligned = env->GetObjectField( player_obj, boundingbox_fid );
	jclass axisaligned_class = env->GetObjectClass( axisaligned );

	const auto minX_fid = env->GetFieldID( axisaligned_class, "field_72340_a", "D" ),
		minY_fid = env->GetFieldID( axisaligned_class, "field_72338_b", "D" ),
		minZ_fid = env->GetFieldID( axisaligned_class, "field_72339_c", "D" );

	const auto maxX_fid = env->GetFieldID( axisaligned_class, "field_72336_d", "D" ),
		maxY_fid = env->GetFieldID( axisaligned_class, "field_72337_e", "D" ),
		maxZ_fid = env->GetFieldID( axisaligned_class, "field_72334_f", "D" );

	env->DeleteLocalRef( axisaligned_class );

	SAxisAlignedBB bb{};
	bb.minX = env->GetDoubleField( axisaligned, minX_fid );
	bb.minY = env->GetDoubleField( axisaligned, minY_fid );
	bb.minZ = env->GetDoubleField( axisaligned, minZ_fid );

	bb.maxX = env->GetDoubleField( axisaligned, maxX_fid );
	bb.maxY = env->GetDoubleField( axisaligned, maxY_fid );
	bb.maxZ = env->GetDoubleField( axisaligned, maxZ_fid );

	return bb;
}

bool c_player::can_entity_be_seen( jobject another_ent ) {
	const bool res = gasper::instance->CallMethod<bool>( player_obj, "func_70685_l", "(Lnet/minecraft/entity/Entity;)Z", another_ent );
	return res;
}

bool c_player::isOnGround( ) {
	jfieldID onground_fid = gasper::instance->get_env( )->GetFieldID( gasper::instance->get_env( )->GetObjectClass( player_obj ), "field_70122_E", "Z" );

	return gasper::instance->get_env( )->GetBooleanField( player_obj, onground_fid );
}

bool c_player::isSprinting( ) {
	const bool ret = gasper::instance->CallMethod<bool>( player_obj, "func_70051_ag", "()Z" );
	return ret;
}

bool c_player::isInWater( ) {
	const bool ret = gasper::instance->CallMethod<bool>( player_obj, "func_70090_H", "()Z" );
	return ret;
}

bool c_player::isSneaking( ) {
	const bool ret = gasper::instance->CallMethod<bool>( player_obj, "func_70093_af", "()Z" );
	return ret;
}

float c_player::get_health( ) {
	jmethodID get_health_mid = gasper::instance->get_env( )->GetMethodID( gasper::instance->get_env( )->GetObjectClass( player_obj ), "func_110143_aJ", "()F" );

	return gasper::instance->get_env( )->CallFloatMethod( player_obj, get_health_mid );
}

float c_player::get_max_health( ) {
	jmethodID get_max_health_mid = gasper::instance->get_env( )->GetMethodID( gasper::instance->get_env( )->GetObjectClass( player_obj ), "func_110138_aP", "()F" );

	return gasper::instance->get_env( )->CallFloatMethod( player_obj, get_max_health_mid );
}

sdk::vec3d c_player::get_position( ) {
	auto player_class = gasper::instance->get_env( )->GetObjectClass( player_obj );

	jfieldID pos_x_fid = gasper::instance->get_env( )->GetFieldID( player_class, "field_70165_t", "D" );
	jfieldID pos_y_fid = gasper::instance->get_env( )->GetFieldID( player_class, "field_70163_u", "D" );
	jfieldID pos_z_fid = gasper::instance->get_env( )->GetFieldID( player_class, "field_70161_v", "D" );


	gasper::instance->get_env( )->DeleteLocalRef( player_class );

	return sdk::vec3d{
		gasper::instance->get_env( )->GetDoubleField( player_obj, pos_x_fid ),
			gasper::instance->get_env( )->GetDoubleField( player_obj, pos_y_fid ),
			gasper::instance->get_env( )->GetDoubleField( player_obj, pos_z_fid )
	};
}

void c_player::set_yaw( float yaw )
{
	auto player_class = gasper::instance->get_env( )->GetObjectClass( player_obj );

	jfieldID yaw_fid;
	if ( !MINECRAFT_1_8 ) {
		yaw_fid = gasper::instance->get_env( )->GetFieldID( player_class, xorstr_( "field_70177_z" ), xorstr_( "F" ) );
	}
	else {
		yaw_fid = gasper::instance->get_env( )->GetFieldID( player_class, xorstr_( "y" ), xorstr_( "F" ) );
	}

	gasper::instance->get_env( )->DeleteLocalRef( player_class );

	gasper::instance->get_env( )->SetFloatField( player_obj, yaw_fid, yaw );
}

float c_player::getRotationYaw( )
{
	auto player_class = gasper::instance->get_env( )->GetObjectClass( player_obj );

	jfieldID yaw_fid;
	if ( !MINECRAFT_1_8 ) {
		yaw_fid = gasper::instance->get_env( )->GetFieldID( player_class, xorstr_( "field_70177_z" ), xorstr_( "F" ) );
	}
	else {
		yaw_fid = gasper::instance->get_env( )->GetFieldID( player_class, xorstr_( "y" ), xorstr_( "F" ) );
	}

	gasper::instance->get_env( )->DeleteLocalRef( player_class );

	return gasper::instance->get_env( )->GetFloatField( player_obj, yaw_fid );
}

void c_player::set_pitch( float pitch )
{
	auto player_class = gasper::instance->get_env( )->GetObjectClass( player_obj );

	jfieldID pitch_fid;
	if ( !MINECRAFT_1_8 ) {
		pitch_fid = gasper::instance->get_env( )->GetFieldID( player_class, xorstr_( "field_70125_A" ), xorstr_( "F" ) );
	}
	else {
		pitch_fid = gasper::instance->get_env( )->GetFieldID( player_class, xorstr_( "z" ), xorstr_( "F" ) );
	}

	gasper::instance->get_env( )->DeleteLocalRef( player_class );

	gasper::instance->get_env( )->SetFloatField( player_obj, pitch_fid, pitch );
}

void c_player::setMoveStrafing( float s ) {
	gasper::instance->SetField<float>( player_obj, "field_70702_br", "F", s );
}

float c_player::get_pitch( )
{
	auto player_class = gasper::instance->get_env( )->GetObjectClass( player_obj );

	jfieldID pitch_fid;
	if ( !MINECRAFT_1_8 ) {
		pitch_fid = gasper::instance->get_env( )->GetFieldID( player_class, xorstr_( "field_70125_A" ), xorstr_( "F" ) );
	}
	else {
		pitch_fid = gasper::instance->get_env( )->GetFieldID( player_class, xorstr_( "z" ), xorstr_( "F" ) );
	}

	gasper::instance->get_env( )->DeleteLocalRef( player_class );

	return gasper::instance->get_env( )->GetFloatField( player_obj, pitch_fid );
}

float c_player::get_height( )
{
	auto player_class = gasper::instance->get_env( )->GetObjectClass( player_obj );

	jfieldID height_fid = gasper::instance->get_env( )->GetFieldID( player_class, "field_70131_O", "F" );

	gasper::instance->get_env( )->DeleteLocalRef( player_class );

	return gasper::instance->get_env( )->GetFloatField( player_obj, height_fid );
}

double c_player::getMotionX( ) {
	const double ret = gasper::instance->GetField<double>( player_obj, "field_70159_w", "D" );
	return ret;
}
double c_player::getMotionY( ) {
	const double ret = gasper::instance->GetField<double>( player_obj, "field_70181_x", "D" );
	return ret;
}
double c_player::getMotionZ( ) {
	const double ret = gasper::instance->GetField<double>( player_obj, "field_70179_y", "D" );
	return ret;
}

void c_player::setMotionX( double x ) {
	gasper::instance->SetField( player_obj, "field_70159_w", "D", x ); //motionX
}

void c_player::setMotionY( double y ) {
	gasper::instance->SetField( player_obj, "field_70181_x", "D", y ); //motionY
}

void c_player::setMotionZ( double z ) {
	gasper::instance->SetField( player_obj, "field_70179_y", "D", z ); //motionZ
}

const std::string_view& c_player::get_display_name( ) {
	const auto env = gasper::instance->get_env( );
	const auto name_func = env->GetMethodID( env->GetObjectClass( player_obj ), "getDisplayName", "()Ljava/lang/String;" );
	const auto ret = (jstring)env->CallObjectMethod( player_obj, name_func );

	const char* jstr = env->GetStringUTFChars( ret, NULL );
	env->ReleaseStringUTFChars( ret, jstr );
	return jstr;
}

double c_player::get_distance_to( std::shared_ptr<c_player> other )
{
	auto pos = get_position( );
	auto entity_pos = other->get_position( );
	return sdk::util::distance( pos.x, pos.y, pos.z, entity_pos.x, entity_pos.y, entity_pos.z );
}
