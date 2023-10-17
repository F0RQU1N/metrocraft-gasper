#pragma once

#include "../minecraft.h"

struct SAxisAlignedBB {
	double minX, minY, minZ,
		maxX, maxY, maxZ;
};

class c_player {
private:
	jobject player_obj;
public:
	c_player(jobject obj);
	~c_player();

	bool is_invisible();
	void set_position( sdk::vec3d vec );
	void setSprinting( bool );
	float getHurtTime();
	float getMoveForward();
	float getMoveStrafing();
	sdk::vec3d get_lasttick_position();
	SAxisAlignedBB get_native_bbox();
	bool can_entity_be_seen(jobject another_ent);
	bool isOnGround();
	bool isSprinting();
	bool isInWater();
	bool isSneaking();
	float get_health();
	float get_max_health();
	void setMoveForward( float );
	sdk::vec3d get_position();
	jobject get_capabilities( );
	void set_yaw(float yaw);
	float getRotationYaw();
	void set_pitch(float pitch);
	void setMoveStrafing(float);
	float get_pitch();
	float get_height();
	double getMotionX( );
	double getMotionY( );
	double getMotionZ( );
	void setMotionX( double );
	void setMotionY( double );
	void setMotionZ( double );
	const std::string_view& get_display_name();
	double get_distance_to(std::shared_ptr<c_player>);

	const jobject get_object()
	{
		return player_obj;
	}
};