(eval-when (:compile-toplevel :execute :load-toplevel)
  (ql:quickload :cl-cpp-generator))

(in-package :cl-cpp-generator)

(defmacro e (&body body)
  `(statements (<< "std::cout" ,@(loop for e in body collect
				      (cond ((stringp e) `(string ,e))
					    (t e))) "std::endl")))
(defparameter *ndk-facts*
    `((10 "bla")
      ))

;; /home/martin/and/android-ndk-r14b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm apk/lib/armeabi-v7a/libhello.so
;; export PATH=$PATH:/data/data/com.termux/files/usr/bin ; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/data/com.termux/files/usr/lib 

(defparameter *callbacks* '((onStart) ;
				  (onResume) ;
				  (onSaveInstanceState :ret "static void*" :args ((outlen :type size_t*)) :body (return nullptr))
				  (onPause) ;
				  (onStop) ;
				  (onDestroy) ;
				  (onWindowFocusChanged :args ((focused :type int)))
			    (onNativeWindowCreated  :args ((w :type ANativeWindow*)) :body (setf g_window w))
			    (onNativeWindowResized  :args ((w :type ANativeWindow*)))
			    (onNativeWindowRedrawNeeded  :args ((w :type ANativeWindow*)))
			    (onNativeWindowDestroyed  :args ((w :type ANativeWindow*)) :body (setf g_window nullptr))
			
			    (onInputQueueCreated :args ((in :type AInputQueue*)) :body (setf g_input_queue in))
				  (onInputQueueDestroyed :args ((in :type AInputQueue*)) :body (setf g_input_queue nullptr))
				  (onContentRectChanged :args ((rect :type "const ARect*")))
				  (onConfigurationChanged )
				  (onLowMemory) ;
				  ))

(let ((code `(with-compilation-unit
		 
		 (include <android/native_activity.h>)
	       (include <android/log.h>)
	       (include <jni.h>)
	       ;(include <string.h>)
	       (include <unistd.h>) ;; usleep
	       ;;(include <android/looper.h>)
	       (include <android/window.h>)
	       (include <android/rect.h>)
	       (include <android/native_window_jni.h>)
	       (include <android/native_window.h>)
	       
	       (decl ((g_input_queue :type AInputQueue* :ctor nullptr)
		      (g_window :type ANativeWindow* :ctor nullptr)))
	       ,@(loop for e in *callbacks*  collect
		      (destructuring-bind (name &key (ret "static void") args (body '(statements))) e
			`(function (,name ((activity :type ANativeActivity*)
					   ,@args) ,ret)
				  (funcall __android_log_print ANDROID_LOG_INFO
					   (string "native-activity")
					   (string ,(format nil "~a %p" name))
					   activity)
				  ,body)))
	       (function (android_app_create ((activity :type ANativeActivity*)
					      (savedState :type void*)
					      (savedStateSize :type size_t))
					     "static void*")
			 (statements ;let ((input_looper :ctor (funcall ALooper_prepare ALOOPER_PREPARE_ALLOW_NON_CALLBACKS)))
			  (while true
			    (statements ;let ((window_initialized_p :type "static bool" :ctor false))
			     (if (with-compilation-unit ;&& (== false window_initialized_p)
				     (!= nullptr g_window))
				  #+nil (statements
				   (setf window_initialized_p true)
				   )
				  (statements
				   (statements ;xif (!= nullptr g_window)
				       (statements
					(funcall __android_log_print ANDROID_LOG_INFO
						 (string "native-activity")
						 (string "draw"))
					(funcall ANativeWindow_setBuffersGeometry g_window 0 0 WINDOW_FORMAT_RGBA_8888)
					(let ((buf :type ANativeWindow_Buffer))
					  (if (<= 0 (funcall ANativeWindow_lock g_window &buf nullptr))
					      (statements
					       (funcall memset buf.bits (hex #xff0a0aaa) (* buf.stride buf.height (funcall sizeof uint32_t)))
					       (let ((p :type char* :ctor (funcall reinterpret_cast<char*> buf.bits)))
						 (dotimes (i 256)
						   (dotimes (j 256)
						     (setf
						      (aref p (+ 0 (* (funcall sizeof uint32_t) (+ (* i buf.stride) j)))) 255
						      (aref p (+ 1 (* (funcall sizeof uint32_t) (+ (* i buf.stride) j)))) 0
						      (aref p (+ 2 (* (funcall sizeof uint32_t) (+ (* i buf.stride) j)))) 0
						      (aref p (+ 3 (* (funcall sizeof uint32_t) (+ (* i buf.stride) j)))) 255))))
					       (funcall ANativeWindow_unlockAndPost g_window)))))))))
			    (if (!= nullptr g_input_queue)
				(statements
				 (while (== 1 (funcall AInputQueue_hasEvents g_input_queue))
				   (let ((event :type AInputEvent*))
				     (funcall AInputQueue_getEvent g_input_queue &event)
				     (funcall __android_log_print ANDROID_LOG_INFO
					      (string "native-activity")
					      (string "event %p %d")
					      event
					      (funcall AInputEvent_getType event))
				     (funcall AInputQueue_finishEvent g_input_queue event 1)
				     ))))
			    (funcall usleep 1000)))
			 (return nullptr))
	       (function (ANativeActivity_onCreate ((activity :type ANativeActivity*)
						      (savedState :type void*)
						      (savedStateSize :type size_t)) void)
		  
			 (funcall __android_log_print ANDROID_LOG_INFO
				  (string "native-activity")
				  (string "%p")
				  activity)
		  (setf ,@(loop for e in *callbacks* appending
			       (destructuring-bind (name &key (ret "static void") args (body '(statements))) e
				 `((slot->value activity->callbacks ,name) ,name))))
		  (setf activity->instance (funcall android_app_create activity savedState savedStateSize)
			)))))
  (write-source "/home/martin/and/src/jni/hello" "cpp" code))


