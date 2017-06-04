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
					       (funcall ANativeWindow_unlockAndPost g_window))))))

(let ((code `(with-compilation-unit
		 
		 (include "android_native_app_glue.h")
	       (include <android/log.h>)
	       ;; (include <jni.h>)
	       ;; (include <android/window.h>)
	       ;; (include <android/rect.h>)
	       ;; (include <android/native_window_jni.h>)
	       ;; (include <android/native_window.h>)
	       
	       (function (drawSomething ((app :type android_app*)) void)
			 (let ((win :ctor app->window))
			   (funcall ANativeWindow_setBuffersGeometry win 0 0 WINDOW_FORMAT_RGBA_8888)
			   (let ((buf :type ANativeWindow_Buffer)
				 )
			     (if (< (funcall ANativeWindow_lock win &buf nullptr) 0)
				 (statements (return)))
			     (funcall memset buf.bits (hex #xff0a0aaa) (* buf.stride buf.height (funcall sizeof uint32_t)))
			     (let ((p :type char* :ctor (funcall reinterpret_cast<char*> buf.bits)))
			       (dotimes (i 256)
				 (dotimes (j 256)
				   (setf
				    (aref p (+ 0 (* (funcall sizeof uint32_t) (+ (* i buf.stride) j)))) (funcall static_cast<char> 255)
				    (aref p (+ 1 (* (funcall sizeof uint32_t) (+ (* i buf.stride) j)))) 0
				    (aref p (+ 2 (* (funcall sizeof uint32_t) (+ (* i buf.stride) j)))) 0
				    (aref p (+ 3 (* (funcall sizeof uint32_t) (+ (* i buf.stride) j)))) (funcall static_cast<char> 255)))))
			     (funcall ANativeWindow_unlockAndPost win))))
	       
	       (function (handle_activity_lifecycle_events ((app :type "struct android_app*")
							    (cmd :type int32_t)) void)
			 (funcall __android_log_print ANDROID_LOG_INFO
					      (string "native-activity")
					      (string "%d: dummy data %d")
					      cmd
					      "*(reinterpret_cast<int*>(app->userData))")
			 (case cmd
			   (APP_CMD_INIT_WINDOW
			    (funcall drawSomething app))))
	       
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

