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


(let ((code `(with-compilation-unit
		 
		 (include "android_native_app_glue.h")
	       (include <android/log.h>)
	       (include <jni.h>)

	       (function (handle_activity_lifecycle_events ((app :type "struct android_app*")
							    (cmd :type int32_t)) void)
			 (funcall __android_log_print ANDROID_LOG_INFO
					      (string "native-activity")
					      (string "%d: dummy data %d")
					      cmd
					      "*(reinterpret_cast<int*>(app->userData))"))
	       
	       (function (android_main ((app :type android_app*))
				       void)
			 (funcall app_dummy)
			 (let ((dummy_data :type int :ctor 111))
			   (setf app->userData &dummy_data))
			 (setf app->onAppCmd handle_activity_lifecycle_events)
			 (while true
			   (let ((events :type int)
				 (source :type "struct android_poll_source*")
				 (ident :type int :ctor (funcall ALooper_pollAll -1 nullptr &events
								 (funcall reinterpret_cast<void**> &source))))
			     (if (<= 0 ident)
				 (statements
				  (funcall source->process app source)))))))))
  (write-source "/home/martin/and/src/jni/hello" "cpp" code))

