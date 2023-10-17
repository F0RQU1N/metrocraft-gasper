#pragma once

#include "minecraft.h"

class Renderer {
public:
	inline static GLint* VIEWPORT;
	inline static GLfloat* MODELVIEW;
	inline static GLfloat* PROJECTION;

	static GLint* Get_VIEWPORT( );

	static GLfloat* Get_MODELVIEW( );

	static GLfloat* Get_PROJECTION( );

	static sdk::vec3d GetRenderPos( );

	static sdk::vec4d W2SMultiply( sdk::vec4d vec, GLfloat mat[ 16 ] )
	{
		return sdk::vec4d(
			vec.x * mat[ 0 ] + vec.y * mat[ 4 ] + vec.z * mat[ 8 ] + vec.w * mat[ 12 ],
			vec.x * mat[ 1 ] + vec.y * mat[ 5 ] + vec.z * mat[ 9 ] + vec.w * mat[ 13 ],
			vec.x * mat[ 2 ] + vec.y * mat[ 6 ] + vec.z * mat[ 10 ] + vec.w * mat[ 14 ],
			vec.x * mat[ 3 ] + vec.y * mat[ 7 ] + vec.z * mat[ 11 ] + vec.w * mat[ 15 ]
		);
	}

	static bool W2S( sdk::vec3d pointInWorld, sdk::vec2d& screen, GLfloat modelView[ 16 ], GLfloat projection[ 16 ], GLfloat viewPort[ 4 ] )
	{
		sdk::vec4d clipSpacePos = W2SMultiply( W2SMultiply( sdk::vec4d( pointInWorld.x, pointInWorld.y, pointInWorld.z, 1.0f ), modelView ), projection );

		sdk::vec3d ndcSpacePos = sdk::vec3d( clipSpacePos.x / clipSpacePos.w, clipSpacePos.y / clipSpacePos.w, clipSpacePos.z / clipSpacePos.w );

		// nPlane = -1, fPlane = 1
		if ( ndcSpacePos.z < -1.0 || ndcSpacePos.z > 1.0 )
		{
			return false;
		}

		screen.x = ( ( ndcSpacePos.x + 1.0f ) / 2.0f ) * viewPort[ 2 ];
		screen.y = ( ( 1.0f - ndcSpacePos.y ) / 2.0f ) * viewPort[ 3 ];
		return true;
	}
	
};