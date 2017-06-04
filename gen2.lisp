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

(defmacro alog (&rest rest)
  `(statements (funcall __android_log_print ANDROID_LOG_INFO
	     (string "native-activity")
	     ,@rest
	     )))

(defmacro aassert (cond)
  `(if (! ,cond)
       (statements
	(funcall __android_log_print ANDROID_LOG_INFO
		 (string "native-activity")
		 (string ,(format nil "assertion ~a failed" cond))))))

(let ((code `(with-compilation-unit
		 
		 (include "android_native_app_glue.h")
	       (include <android/log.h>)
	       ;; (include <jni.h>)
	       ;; (include <android/window.h>)
	       ;; (include <android/rect.h>)
	       ;; (include <android/native_window_jni.h>)
	       ;; (include <android/native_window.h>)
	       (include <android/sensor.h>)
	       (include <cmath>)
	       (include <algorithm>)

	       ,(let ((n 1000))
		  `(with-compilation-unit
		    (enum Constants (M_MAG_N ,n))

		    (decl ((m_previous_x :type int :ctor -1)
			   ((aref m_mag M_MAG_N) :type float :init (list ,@ (loop for i below n collect 0.0)))
			   ((aref m_mag2 M_MAG_N) :type float :init (list ,@ (loop for i below n collect 0.0)))
			   (m_mag_idx :type int :init 0)))))

	       (struct userdata_t ()
		       (decl ((move_x :type int32_t)
			      (sensor_event_queue :type ASensorEventQueue*)
			      (sensor_accelerometer :type "const ASensor*")
			      )))
	       
	       (function (handle_input_events ((app :type android_app*)
					       (event :type AInputEvent*)) int32_t)
			 
			 (let ((etype :ctor (funcall AInputEvent_getType event)))
			   
			   (case etype
			     (AINPUT_EVENT_TYPE_MOTION
			      (let ((action :ctor (funcall AMotionEvent_getAction event))
				    (pointer_index :type int32_t :ctor (>> (& action AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
									   AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT))
				    (pos_x :type int32_t :ctor (funcall AMotionEvent_getX event pointer_index)))
				(if (== AMOTION_EVENT_ACTION_MOVE action)
				    (statements
				     
				     (let ((move_x :ctor (- pos_x m_previous_x))
					   (data :type userdata_t* :ctor (funcall reinterpret_cast<userdata_t*> app->userData))
					   )
				       (funcall __android_log_print ANDROID_LOG_INFO
					      (string "native-activity")
					      (string "motion %d")
					      move_x)
				       (setf data->move_x move_x
					     app->redrawNeeded 1)
				       )))
				(setf m_previous_x pos_x))
			      (return 1) ;; stop event processing
			      )))
			 (return 1))
	       
	       (function (drawSomething ((app :type android_app*)) void)
			 (let ((win :ctor app->window)) 
			   (funcall ANativeWindow_setBuffersGeometry win 0 0 WINDOW_FORMAT_RGBA_8888)
			   (let ((buf :type ANativeWindow_Buffer)
				 )
			     (if (< (funcall ANativeWindow_lock win &buf nullptr) 0)
				 (statements (return)))
			     (funcall memset buf.bits (hex #xff0a0aaa) (* buf.stride buf.height (funcall sizeof uint32_t)))
			     (let ((p :type char* :ctor (funcall reinterpret_cast<char*> buf.bits))
				   (x :type "static int" :ctor 0))
			       (+= x (slot->value (funcall reinterpret_cast<userdata_t*> app->userData) move_x))
			       #+nil (dotimes (i 256)
				 (dotimes (j 256)
				   (let ((pos :ctor (* (funcall sizeof uint32_t) (+ (* (% (+ j x)
											  buf.stride)
										       buf.stride) i))))
				    (setf
				     (aref p (+ 0 pos)) (funcall static_cast<char> 128)
				     (aref p (+ 1 pos)) (funcall static_cast<char> 255)
				     (aref p (+ 2 pos)) 0
				     (aref p (+ 3 pos)) (funcall static_cast<char> 255)))))
			       (let ((mi :ctor (aref m_mag2 0))
				     (ma :ctor (aref m_mag2 0)))
				 (dotimes (i M_MAG_N)
				   (setf mi (funcall "std::min" (aref m_mag2 i) mi))
				   (setf mi (funcall "std::max" (aref m_mag2 i) mi)))
				 (dotimes (y M_MAG_N)
				   (dotimes (l 5)
				    (dotimes (k 5)
				      (let ((xx :type int :ctor (funcall static_cast<int> (/ (* 1s0 (- buf.stride 1) (- (aref m_mag2 y) mi))
											     (- ma mi))))
					    (pos :ctor (* (funcall sizeof uint32_t) (+ (* (% (+ y x l) buf.height)
											  buf.stride)
										       (% (+ k xx) buf.stride)))))
					(setf
					 (aref p (+ 0 pos)) (funcall static_cast<char> 255)
					 (aref p (+ 1 pos)) (funcall static_cast<char> 0)
					 (aref p (+ 2 pos)) 0
					 (aref p (+ 3 pos)) (funcall static_cast<char> 255))
				     
					))))))
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
			    (funcall drawSomething app)
			    )
			   (APP_CMD_GAINED_FOCUS
			    (let ((data :ctor (funcall reinterpret_cast<userdata_t*> app->userData)))
			      (if (!= nullptr data->sensor_accelerometer)
				  (statements
				   (funcall ASensorEventQueue_enableSensor data->sensor_event_queue data->sensor_accelerometer)
				   (funcall ASensorEventQueue_setEventRate data->sensor_event_queue data->sensor_accelerometer
					    (* (/ (raw "1000L") 60)
					       1000))))))
			   (APP_CMD_LOST_FOCUS
			    (let ((data :ctor (funcall reinterpret_cast<userdata_t*> app->userData)))
			      (if (!= nullptr data->sensor_accelerometer)
				  (statements
				   (funcall ASensorEventQueue_disableSensor data->sensor_event_queue data->sensor_accelerometer)
				  ))))))
	       
	       (function (android_main ((app :type android_app*))
				       void)
			 (funcall app_dummy)
			 (let ((data :type userdata_t :ctor (list 0))
			       (sensor_manager :ctor (funcall ASensorManager_getInstance))
			       (looper :ctor (funcall ALooper_prepare ALOOPER_PREPARE_ALLOW_NON_CALLBACKS)))
			   (macroexpand (aassert (!= nullptr looper)))
			   (macroexpand (aassert (!= nullptr sensor_manager)))
			   (setf data.sensor_accelerometer (funcall ASensorManager_getDefaultSensor sensor_manager ASENSOR_TYPE_ACCELEROMETER))
			  
			   (macroexpand (aassert (!= nullptr data.sensor_accelerometer)))
			   (setf data.sensor_accelerometer (funcall ASensorManager_getDefaultSensor sensor_manager ASENSOR_TYPE_ACCELEROMETER))
			   
			   (macroexpand (aassert (!= nullptr data.sensor_accelerometer)))
			   (setf data.sensor_event_queue (funcall ASensorManager_createEventQueue sensor_manager looper LOOPER_ID_USER nullptr nullptr))
			   
			   (macroexpand (aassert (!= nullptr data.sensor_event_queue)))
			   (setf app->userData &data
				 app->onAppCmd handle_activity_lifecycle_events
				 app->onInputEvent handle_input_events)
			   (while (== 0 app->destroyRequested)
			     (let ((events :type int)
				   (source :type android_poll_source*)
				   (ident :type int :ctor (funcall ALooper_pollAll (? app->redrawNeeded 0 -1) nullptr &events
								   (funcall reinterpret_cast<void**> &source))))
			       (if (<= 0 ident)
				   (statements
				    (case ident
				      (LOOPER_ID_USER
					 (let ((event :type ASensorEvent))
					   (while (< 0 (funcall ASensorEventQueue_getEvents data.sensor_event_queue &event 1))
					     #+nil (statements ;let ((time :ctor (funcall get_time)))
					       (funcall __android_log_print ANDROID_LOG_INFO
							(string "native-activity")
							(string "accel %f")
							;time
							event.acceleration.x))
					     (let ((mag :ctor (funcall "std::sqrt" (+ (* event.acceleration.x  event.acceleration.x )
								  (* event.acceleration.y  event.acceleration.y )
								  (* event.acceleration.z  event.acceleration.z )))))
					       (setf (aref m_mag m_mag_idx) mag
						     m_mag_idx (% (+ m_mag_idx 1)
								  M_MAG_N))
					       (if (== 0 m_mag_idx)
						   (statements
						    (macroexpand (alog (string "a: %f %f %f") (aref m_mag 0) (aref m_mag 1) (aref m_mag 2)))
						    (dotimes (i M_MAG_N)
						      (setf (aref m_mag2 i) (aref m_mag i)))
						    (setf app->redrawNeeded 1)))))))
				      (t
				       (statements
					(if (!= nullptr source)
					    (funcall source->process app source))
					(if app->redrawNeeded
					    (funcall drawSomething app))))))))))))))
  (write-source "/home/martin/and/src/jni/hello" "cpp" code))

