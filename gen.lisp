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
			    (onNativeWindowCreated  :args ((w :type ANativeWindow*)))
			    (onNativeWindowResized  :args ((w :type ANativeWindow*)))
			    (onNativeWindowRedrawNeeded  :args ((w :type ANativeWindow*)))
			    (onNativeWindowDestroyed  :args ((w :type ANativeWindow*)))
			
			    (onInputQueueCreated :args ((in :type AInputQueue*)))
				  (onInputQueueDestroyed :args ((in :type AInputQueue*)))
				  (onContentRectChanged :args ((rect :type "const ARect*")))
				  (onConfigurationChanged )
				  (onLowMemory) ;
				  ))

(let ((code `(with-compilation-unit
		 
		 (include <android/native_activity.h>)
	       (include <android/log.h>)
	       (include <jni.h>)
	       (include <string.h>)
	       ,@(loop for e in *callbacks*  collect
		      (destructuring-bind (name &key (ret "static void") args (body '(statements))) e
			`(function (,name ((activity :type ANativeActivity*)
					   ,@args) ,ret)
				  (funcall __android_log_print ANDROID_LOG_INFO
					   (string "native-activity")
					   (string ,(format nil "~a %p" name))
					   activity)
				  ,body)))
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
		  (setf activity->instance nullptr)))))
  (write-source "/home/martin/and/src/jni/hello" "cpp" code))


