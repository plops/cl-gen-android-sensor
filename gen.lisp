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
(let ((code `(with-compilation-unit
		 
		 (include <android/native_activity.h>)
	       (include <android/log.h>)
	       (include <jni.h>)
	       ,@(loop for e in '((onStart) ;
				  (onResume) ;
				  (onSaveInstanceState :ret "static void*" :args ((outlen :type size_t*)) :body (return nullptr))
				  (onPause)
				  (onStop)
				  (onDestroy) ;
				  (onWindowFocusChanged)
				  (onNativeWindowCreated)
				  (onNativeWindowRedrawNeeded)
				  (onNativeWindowDestroyed)
				  (onInputQueueCreated)
				  (onInputQueueDestroyed)
				  (onContentRectChanged)
				  (onConfigurationChanged)
				  (onLowMemory)) collect
		      `(function (,e ((activity :type android_activity_t*)))
				 (funcall __android_log_print ANDROID_LOG_INFO
					  (string ,(format nil "native-activity ~a" e)))))
	       (function (ANativeActivity_onCreate ((activity :type ANativeActivity*)
						      (savedState :type void*)
						      (savedStateSize :type size_t)) void)
		  
			 (funcall __android_log_print ANDROID_LOG_INFO
				  (string "native-activity")
				  activity)
		  (setf ,@(loop for e in '(onStart onResume onSaveInstanceState
					   onPause onStop onDestroy
					   onWindowFocusChanged onNativeWindowCreated
					   onNativeWindowRedrawNeeded onNativeWindowDestroyed
					   onInputQueueCreated onInputQueueDestroyed
					   onContentRectChanged onConfigurationChanged onLowMemory
					   ) appending
			       `((slot->value activity->callbacks ,e) ,e)))
		  (setf activity->instance nullptr)))))
  (write-source "/home/martin/and/src/jni/hello" "cpp" code))


