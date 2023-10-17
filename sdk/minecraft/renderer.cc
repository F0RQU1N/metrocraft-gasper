#include "renderer.h"

#include "../../main/gasper.h"

sdk::vec3d Renderer::GetRenderPos( ) {
	auto env = gasper::instance->get_env( );
	jclass cl = gasper::instance->find_class( "net.minecraft.client.renderer.entity.RenderManager" );

	auto field_78725_b = env->GetStaticFieldID( cl, "field_78725_b", "D" );
	auto field_78726_c = env->GetStaticFieldID( cl, "field_78726_c", "D" );
	auto field_78723_d = env->GetStaticFieldID( cl, "field_78723_d", "D" );

	auto ret = sdk::vec3d{ env->GetStaticDoubleField( cl, field_78725_b ), env->GetStaticDoubleField( cl, field_78726_c ), env->GetStaticDoubleField( cl, field_78723_d ) };
	return ret;
}

GLint* Renderer::Get_VIEWPORT( ) {
	jclass CActiveRenderInfo = gasper::instance->find_class( "net.minecraft.client.renderer.ActiveRenderInfo" );
	jfieldID FIDVIEWPORT = gasper::instance->get_env()->GetStaticFieldID( CActiveRenderInfo, "field_74597_i", "Ljava/nio/IntBuffer;" );
	jobject OVIEWPORT = gasper::instance->get_env( )->GetStaticObjectField( CActiveRenderInfo, FIDVIEWPORT );
	gasper::instance->get_env( )->DeleteLocalRef( CActiveRenderInfo );
	jclass CIntBuffer = gasper::instance->find_class( "java.nio.IntBuffer" );
	jmethodID MIDget = gasper::instance->get_env( )->GetMethodID( CIntBuffer, "get", "(I)I" );
	for ( int i = 0; i < 16; i++ )
	{
		VIEWPORT[ i ] = gasper::instance->get_env( )->CallIntMethod( OVIEWPORT, MIDget, i );
	}
	gasper::instance->get_env( )->DeleteLocalRef( OVIEWPORT );
	gasper::instance->get_env( )->DeleteLocalRef( CIntBuffer );
	return VIEWPORT;
}

GLfloat* Renderer::Get_MODELVIEW( ) {
	jclass CActiveRenderInfo = gasper::instance->find_class( "net.minecraft.client.renderer.ActiveRenderInfo" );
	jfieldID FIDMODELVIEW = gasper::instance->get_env()->GetStaticFieldID( CActiveRenderInfo, "field_74594_j", "Ljava/nio/FloatBuffer;" );
	jobject OMODELVIEW = gasper::instance->get_env()->GetStaticObjectField( CActiveRenderInfo, FIDMODELVIEW );
	gasper::instance->get_env()->DeleteLocalRef( CActiveRenderInfo );
	jclass CFloatBuffer = gasper::instance->find_class( "java.nio.FloatBuffer" );
	jmethodID MIDget = gasper::instance->get_env()->GetMethodID( CFloatBuffer, "get", "(I)F" );
	for ( int i = 0; i < 16; i++ )
	{
		MODELVIEW[ i ] = gasper::instance->get_env()->CallFloatMethod( OMODELVIEW, MIDget, i );
	}
	gasper::instance->get_env()->DeleteLocalRef( OMODELVIEW );
	gasper::instance->get_env()->DeleteLocalRef( CFloatBuffer );
	return MODELVIEW;
}

GLfloat* Renderer::Get_PROJECTION( ) {
	jclass CActiveRenderInfo = gasper::instance->find_class( "net.minecraft.client.renderer.ActiveRenderInfo" );
	jfieldID FIDPROJECTION = gasper::instance->get_env()->GetStaticFieldID( CActiveRenderInfo, "field_74595_k", "Ljava/nio/FloatBuffer;" );
	jobject OPROJECTION = gasper::instance->get_env()->GetStaticObjectField( CActiveRenderInfo, FIDPROJECTION );
	gasper::instance->get_env()->DeleteLocalRef( CActiveRenderInfo );
	jclass CFloatBuffer = gasper::instance->find_class( "java.nio.FloatBuffer" );
	jmethodID MIDget = gasper::instance->get_env()->GetMethodID( CFloatBuffer, "get", "(I)F" );
	for ( int i = 0; i < 16; i++ )
	{
		PROJECTION[ i ] = gasper::instance->get_env()->CallFloatMethod( OPROJECTION, MIDget, i );
	}
	gasper::instance->get_env()->DeleteLocalRef( OPROJECTION );
	gasper::instance->get_env()->DeleteLocalRef( CFloatBuffer );
	return PROJECTION;
}