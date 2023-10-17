#pragma once

#include "../sdk/includes.h"

namespace gasper {
	class c_gasper {
	private:
		JNIEnv* env;
		jvmtiEnv* jvmti_env;
		JavaVM* vm;
		jmethodID findclass_md;
		jobject classloader_obj;
	public:
		bool b_fullbright = false;
		bool b_speedhack = false;
		float f_shmp = false;
		bool b_running;
		jobject theMc;
		
		bool b_open;

		HMODULE _module;

		/// Function to try and attach to the JVM
		bool attach( );

		/// Run the cheat 
		void run( );

		/// Dispose of cheat
		void dispose( );

		/// Get the launchwrapper for forge support 
		void get_launchwrapper( );

		/// Hook necessary functions
		void hook( );

		const auto get_env( ) {
			return env;
		}

		const auto get_vm( ) {
			return vm;
		}

		/// Inline function to get class
		inline jclass find_class( const char* name ) {
			jstring jname = get_env( )->NewStringUTF( name );

			jclass cls = reinterpret_cast<jclass>( get_env( )->CallObjectMethod( classloader_obj, findclass_md, jname ) );

			get_env( )->DeleteLocalRef( jname );

			wrapper::handle_issue( name, cls );

			return cls;
		}

		inline jobject find_field( jclass klass, const char* field_name ) {
			auto javaclass = env->FindClass( "java/lang/Class" );
			jmethodID getterID = env->GetMethodID( javaclass, "getFields", "()[Ljava/lang/reflect/Field;" );
			jobjectArray fieldsarray = (jobjectArray)env->CallObjectMethod( klass, getterID );

			jclass field_class = env->FindClass( "java/lang/reflect/Field" );
			const auto leng = env->GetArrayLength( fieldsarray );
			for ( auto i = 0; i < leng; ++i ) {
				jobject field_element = env->GetObjectArrayElement( fieldsarray, i );
				jstring _jfield_name = (jstring)env->CallObjectMethod( field_element, env->GetMethodID( field_class, "getName", "()Ljava/lang/String;" ) );
				const char* _field_name = env->GetStringUTFChars( _jfield_name, nullptr );
				env->ReleaseStringUTFChars( _jfield_name, _field_name );

				if ( strstr( _field_name, field_name ) ) {
					return field_element;
				}
			}

			return nullptr;
		}
		
		inline jobject find_field_by_type( jclass klass, const char* type_name ) {
			auto javaclass = env->FindClass( "java/lang/Class" );
			jmethodID getterID = env->GetMethodID( javaclass, "getFields", "()[Ljava/lang/reflect/Field;" );
			jobjectArray fieldsarray = (jobjectArray)env->CallObjectMethod( klass, getterID );

			jclass field_class = env->FindClass( "java/lang/reflect/Field" );
			const auto leng = env->GetArrayLength( fieldsarray );
			for ( auto i = 0; i < leng; ++i ) {
				jobject field_element = env->GetObjectArrayElement( fieldsarray, i );
				jstring _jfield_name = (jstring)env->CallObjectMethod( field_element, env->GetMethodID( field_class, "getName", "()Ljava/lang/String;" ) );
				const char* _field_name = env->GetStringUTFChars( _jfield_name, nullptr );
				env->ReleaseStringUTFChars( _jfield_name, _field_name );

				if ( strstr( _field_name, type_name ) ) {
					return field_element;
				}
			}

			return nullptr;
		}
		template<class T, class... Args> constexpr T CallMethod( _In_ jobject _JavaMirror,
		_In_ std::string _MethodName, _In_ std::string _MethodSignature, _In_ const Args... _Args ) {
			if constexpr ( std::is_same_v<T, void> ) {
				jmethodID mid = env->GetMethodID( env->GetObjectClass( _JavaMirror ), _MethodName.c_str( ), _MethodSignature.c_str( ) );
				//be sure we HAVE passed any argument
				if constexpr ( sizeof...( _Args ) >= 1 ) {
					env->CallVoidMethod( _JavaMirror, mid, _Args... );
				}
				else {
					env->CallVoidMethod( _JavaMirror, mid );
				}
				return T( );
			}
			else {
				jmethodID mid = env->GetMethodID( env->GetObjectClass( _JavaMirror ), _MethodName.c_str( ), _MethodSignature.c_str( ) );
				if constexpr ( std::is_same_v<T, double> ) {
					if constexpr ( sizeof...( _Args ) >= 1 ) {
						return env->CallDoubleMethod( _JavaMirror, mid, _Args... );
					}
					else {
						return env->CallDoubleMethod( _JavaMirror, mid );
					}
				}
				if constexpr ( std::is_same_v<T, float> ) {
					if constexpr ( sizeof...( _Args ) >= 1 ) {
						return env->CallFloatMethod( _JavaMirror, mid, _Args... );
					}
					else {
						return env->CallFloatMethod( _JavaMirror, mid );
					}
				}
				if constexpr ( std::is_same_v<T, long long> ) {
					if constexpr ( sizeof...( _Args ) >= 1 ) {
						return env->CallLongMethod( _JavaMirror, mid, _Args... );
					}
					else {
						return env->CallLongMethod( _JavaMirror, mid );
					}
				}
				if constexpr ( std::is_same_v<T, int> || std::is_same_v<T, jint> ) {
					if constexpr ( sizeof...( _Args ) >= 1 ) {
						return env->CallIntMethod( _JavaMirror, mid, _Args... );
					}
					else {
						return env->CallIntMethod( _JavaMirror, mid );
					}
				}
				if constexpr ( std::is_same_v<T, byte> || std::is_same_v<T, jbyte> ) {
					if constexpr ( sizeof...( _Args ) >= 1 ) {
						return env->CallByteMethod( _JavaMirror, mid, _Args... );
					}
					else {
						return env->CallByteMethod( _JavaMirror, mid );
					}
				}
				if constexpr ( std::is_same_v<T, short> ) {
					if constexpr ( sizeof...( _Args ) >= 1 ) {
						return env->CallShortMethod( _JavaMirror, mid, _Args... );
					}
					else {
						return env->CallShortMethod( _JavaMirror, mid );
					}
				}
				if constexpr ( std::is_same_v<T, bool> || std::is_same_v<T, boolean> || std::is_same_v<T, jboolean> ) {
					if constexpr ( sizeof...( _Args ) >= 1 ) {
						return env->CallBooleanMethod( _JavaMirror, mid, _Args... );
					}
					else {
						return env->CallBooleanMethod( _JavaMirror, mid );
					}
				}
				if constexpr ( std::is_same_v<T, char> || std::is_same_v<T, jchar> ) {
					if constexpr ( sizeof...( _Args ) >= 1 ) {
						return env->CallCharMethod( _JavaMirror, mid, _Args... );
					}
					else {
						return env->CallCharMethod( _JavaMirror, mid );
					}
				}
				if constexpr ( std::is_same_v<T, jobject> ) {
					if constexpr ( sizeof...( _Args ) >= 1 ) {
						return env->CallObjectMethod( _JavaMirror, mid, _Args... );
					}
					else {
						return env->CallObjectMethod( _JavaMirror, mid );
					}
				}
			}

			return T( );
		}

		template<class T> constexpr T GetField( _In_ jobject _JavaMirror,
		_In_ std::string _FieldName, _In_ std::string _FieldSignature ) {
			jfieldID fid = env->GetFieldID( env->GetObjectClass( _JavaMirror ), _FieldName.c_str( ), _FieldSignature.c_str( ) );
			
			if constexpr ( std::is_same_v<T, double> ) {
				return env->GetDoubleField( _JavaMirror, fid );
			}
			if constexpr ( std::is_same_v<T, float> ) {
				return env->GetFloatField( _JavaMirror, fid );
			}
			if constexpr ( std::is_same_v<T, long long> ) {
				return env->GetLongField( _JavaMirror, fid );
			}
			if constexpr ( std::is_same_v<T, int> ) {
				return env->GetIntField( _JavaMirror, fid );
			}
			if constexpr ( std::is_same_v<T, byte> || std::is_same_v<T, jbyte> ) {
				return env->GetByteField( _JavaMirror, fid );
			}
			if constexpr ( std::is_same_v<T, bool> || std::is_same_v<T, boolean> || std::is_same_v<T, jboolean> ) {
				return env->GetBooleanField( _JavaMirror, fid );
			}
			if constexpr ( std::is_same_v<T, char> ) {
				return env->GetCharField( _JavaMirror, fid );
			}
			if constexpr ( std::is_same_v<T, short> ) {
				return env->GetShortField( _JavaMirror, fid );
			}
			if constexpr ( std::is_same_v<T, jobject> ) {
				return env->GetObjectField( _JavaMirror, fid );
			}

			return T( );
		}

		template<class T> constexpr void SetField( _In_ jobject _JavaMirror,
	_In_ std::string _FieldName, _In_ std::string _FieldSignature, const T val ) {
			jfieldID fid = env->GetFieldID( env->GetObjectClass( _JavaMirror ), _FieldName.c_str( ), _FieldSignature.c_str( ) );

			if constexpr ( std::is_same_v<T, double> ) {
				return env->SetDoubleField( _JavaMirror, fid, val );
			}
			if constexpr ( std::is_same_v<T, float> ) {
				return env->SetFloatField( _JavaMirror, fid, val );
			}
			if constexpr ( std::is_same_v<T, long long> ) {
				return env->SetLongField( _JavaMirror, fid, val );
			}
			if constexpr ( std::is_same_v<T, int> ) {
				return env->SetIntField( _JavaMirror, fid, val );
			}
			if constexpr ( std::is_same_v<T, byte> || std::is_same_v<T, jbyte> ) {
				return env->SetByteField( _JavaMirror, fid, val );
			}
			if constexpr ( std::is_same_v<T, bool> || std::is_same_v<T, boolean> || std::is_same_v<T, jboolean> ) {
				return env->SetBooleanField( _JavaMirror, fid, val );
			}
			if constexpr ( std::is_same_v<T, char> ) {
				return env->SetCharField( _JavaMirror, fid, val );
			}
			if constexpr ( std::is_same_v<T, short> ) {
				return env->SetShortField( _JavaMirror, fid, val );
			}
			if constexpr ( std::is_same_v<T, jobject> ) {
				return env->SetObjectField( _JavaMirror, fid, val );
			}
		}
	};

	namespace hooks {
		/// Function template of SwapBuffers
		using swap_buffers_fn = int( __stdcall* )( HDC );

		/// Store pointer to the original SwapBuffers function
		inline swap_buffers_fn oswap_buffers = nullptr;

		/// Simple glcontext wrapper to make life easy
		extern std::shared_ptr<wrapper::c_context> gl_context;

		/// The actual hook
		int __stdcall swap_buffers_hk( HDC );

		/// Original WndProc of the Window we're hooking
		inline WNDPROC original_wndproc = nullptr;

		/// Actual "hook", this really isn't a hook but it's whatever
		long __stdcall wndproc_hk( const HWND hwnd, unsigned int usermsg, uintptr_t wparam, long lparam );
	}

	extern std::unique_ptr<c_gasper> instance;
}
