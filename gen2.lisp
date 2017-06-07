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
;; gyro docs https://crypto.stanford.edu/gyrophone/files/gyromic.pdf
;; /home/martin/and/android-ndk-r14b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm apk/lib/armeabi-v7a/libhello.so
;; export PATH=$PATH:/data/data/com.termux/files/usr/bin ; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/data/com.termux/files/usr/lib 
(defmacro benchmark (&body body)
  `(let ((start :ctor (funcall current_time)))
     ,@body
     
     (macroexpand (alog (string "time: %f ms") (* (/ 1.0 1000.0) (- (funcall current_time) start))))
     ))

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
(defun rev (x nn)
  (let ((n (floor (log nn 2)))
      (res 0))
  (dotimes (i n)
    (setf (ldb (byte 1 i) res) (ldb (byte 1 (- n 1 i)) x)))
  res))



(let* ((n 1024)
      (code `(with-compilation-unit
		 
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
	       (include <array>)
	       (include <complex>)
               (include <sys/time.h>) ;; gettimeofday

	       (with-compilation-unit
		   (enum Constants (M_MAG_N ,n))

		 (decl ((m_previous_x :type int :ctor -1)
			((aref m_mag M_MAG_N) :type float :init (list ,@ (loop for i below n collect 0.0)))
			((aref m_mag2 M_MAG_N) :type float :init (list ,@ (loop for i below n collect 0.0)))
			(m_mag_idx :type int :init 0)
			(m_fft_in :type "std::array<std::complex<float>,M_MAG_N>"  :init (list (list ,@ (loop for i below n collect 0.0))))
			(m_fft_out :type "std::array<std::complex<float>,M_MAG_N>"  :init (list (list ,@ (loop for i below n collect 0.0))))
			(m_fft_out_mag :type "std::array<float,M_MAG_N>" :init (list (list ,@ (loop for i below n collect 0.0))))
			)))
	        (function (current_time () "static inline uint64_t")
                                         
                                         (let ((tv :type "struct timeval"))
                                           (funcall gettimeofday &tv nullptr)
                                           (return (+ (* tv.tv_sec 1000000)
                                                      tv.tv_usec))))

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
				    (pos_x :type int32_t :ctor (funcall AMotionEvent_getY event pointer_index)))
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

	       (function (set_pixel ((buf :type "ANativeWindow_Buffer&")
				     (x :type int)
				     (y :type int)
				     (r :type int)
				     (g :type int)
				     (b :type int)
				     (a :type int)) "void inline")
			 (let ((p :type char* :ctor (funcall reinterpret_cast<char*> buf.bits))
			       (pos :ctor (* (funcall sizeof uint32_t) 
					     #+nil
					     (+ (* (% y buf.height)
						   buf.stride)
						(% x buf.stride))
					     
					     (+ (* y  buf.stride)
						x)

					     )))
			   (setf
			    (aref p (+ 0 pos)) (funcall static_cast<char> r)
			    (aref p (+ 1 pos)) (funcall static_cast<char> g)
			    (aref p (+ 2 pos)) (funcall static_cast<char> b)
			    (aref p (+ 3 pos)) (funcall static_cast<char> a))))

	       (function (draw_line ((buf :type ANativeWindow_Buffer&)
				     (x1 :type float)
				     (y1 :type float)
				     (x2 :type float)
				     (y2 :type float)
				     (r :type int)
				     (g :type int)
				     (b :type int)
				     (a :type int))
				    void)
			 (let ((steep :ctor (< (funcall "std::abs" (- x2 x1))
					       (funcall "std::abs" (- y2 y1)))))
			   (if steep
			       (statements
				(funcall "std::swap"  x1 y1)
				(funcall "std::swap" x2 y2)
				))
			   (if (< x2 x1)
			       (statements
				(funcall "std::swap" x1 x2)
				(funcall "std::swap" y1 y2)))
			   (let ((dx :ctor (- x2 x1))
				 (dy :ctor (funcall "std::abs" (- y2 y1)))
				 (err :ctor (/ dx 2.0))
				 (ystep :type "const int" :ctor (? (< y1 y2) 1 -1))
				 (y :type int :ctor (funcall static_cast<int> y1))
				 (maxX :type "const int" :ctor (funcall static_cast<int> x2)))
			     (for ((x (funcall static_cast<int> x1) :type int)
				   (< x maxX)
				   (+= x 1))
				  (if steep
				      (funcall set_pixel buf y x r g b a)
				      (funcall set_pixel buf x y r g b a))
				  (-= err dy)
				  (if (< err 0)
				      (statements
				       (+= y ystep)
				       (+= err dx)))))))
	       
	       (function (drawSomething ((app :type android_app*)) void)
			 (let ((win :ctor app->window)) 
			   (funcall ANativeWindow_setBuffersGeometry win 0 0 WINDOW_FORMAT_RGBA_8888)
			   (let ((buf :type ANativeWindow_Buffer)
				 )
			     (if (< (funcall ANativeWindow_lock win &buf nullptr) 0)
				 (statements (return)))
			     (funcall memset buf.bits (hex #xffffffff) (* buf.stride buf.height (funcall sizeof uint32_t)))
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
			       #+nil (dotimes (i 20)
				       (funcall draw_line  buf 100 (* 10 i) 500 (* 5 i)))
			       (let ((mi :ctor (aref m_mag2 0))
				     (ma :ctor (aref m_mag2 0)))
				 (dotimes (i M_MAG_N)
				   (setf mi (funcall "std::min" (aref m_mag2 i) mi))
				   (setf ma (funcall "std::max" (aref m_mag2 i) ma)))
					;(macroexpand (alog (string "w=%d h=%d") buf.stride buf.height)) ;; w=720 h=1280
				 (dotimes (y (- M_MAG_N 1))
				   (let ((xx1 :type int :ctor (funcall static_cast<int> (+ .2s0 (/ (* .6s0 (- buf.stride 1)
												      (- (aref m_mag2 (+ y 1)) mi))
												   (- ma mi)))))
					 (xx0 :type int :ctor (funcall static_cast<int> (+ .2s0 (/ (* .6s0 (- buf.stride 1)
												      (- (aref m_mag2 (+ y 0)) mi))
												   (- ma mi)))))
					 (yy1 :ctor (* (funcall sizeof uint32_t) (% (+ (+ y 1)  ) buf.height)))
					 (yy0 :ctor (* (funcall sizeof uint32_t) (% (+ (+ y 0)  ) buf.height))))
				     (funcall draw_line buf xx0 (/ yy0 4) xx1 (/ yy1 4) 255 0 0 255)
				     
				     ))
				 (let ((mi :ctor (aref m_fft_out_mag 0))
				       (ma :ctor (aref m_fft_out_mag 0)))
				   (dotimes (i (funcall m_fft_out_mag.size))
				     (setf mi (funcall "std::min" mi (aref m_fft_out_mag i))
					   ma (funcall "std::max" ma (aref m_fft_out_mag i))))
				   (dotimes (y (- (funcall m_fft_out_mag.size) 1))
				     (let ((xx1 :type int :ctor (funcall static_cast<int> (+ .2s0 (/ (* .6s0 (- buf.stride 1)
													(- (aref m_fft_out_mag (+ y 1)) mi))
												     (- ma mi)))))
					   (xx0 :type int :ctor (funcall static_cast<int> (+ .2s0 (/ (* .6s0 (- buf.stride 1)
													(- (aref m_fft_out_mag (+ y 0)) mi))
												     (- ma mi)))))
					   (yy1 :ctor (* (funcall sizeof uint32_t) (% (+ (+ y 1)) buf.height)))
					   (yy0 :ctor (* (funcall sizeof uint32_t) (% (+ (+ y 0)) buf.height))))
				       (funcall draw_line buf xx0 (/ yy0 4) xx1 (/ yy1 4) 0 0 0 255))))))
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
					    (* (/ (raw "1000L") 5000 ; 60
						  )					       1000))))))
			   (APP_CMD_LOST_FOCUS
			    (let ((data :ctor (funcall reinterpret_cast<userdata_t*> app->userData)))
			      (if (!= nullptr data->sensor_accelerometer)
				  (statements
				   (funcall ASensorEventQueue_disableSensor data->sensor_event_queue data->sensor_accelerometer)
				   ))))))
	       #+nil
	       (function (ft ((in :type "const std::array<std::complex<float>, N > &" )
			      (out :type "std::array<std::complex<float>, N > &" ))
			     "template<std::size_t N> void"
			     )
			 (dotimes (k N)
			   (setf (aref out k) 0))
			 (dotimes (k N)
			   (dotimes (n N)
			     (+= (aref out k) (* (funcall "std::exp"
							  (funcall "std::complex<float>"
								   0s0
								   (/ (* M_PI -2 k n)
								      N)))
						 (aref in n))))))
	       
	       ;; https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
               (function  (bit_reverse_copy ((in :type "const std::array<std::complex<float>, N > &")
                                             (out :type "std::array<std::complex<float>, N > &"))
                                            "template<std::size_t N > void")
                          (setf ,@(loop for i below n appending
				       `((aref out ,(rev i n)) (aref in ,i)))))
               (function 
		(fft ((in :type "const std::array<std::complex<float>, N > &")
		      (out :type "std::array<std::complex<float>, N > &"))
		     "template<std::size_t N > void")
		(funcall bit_reverse_copy in out)
		,@(loop for s from 1 upto (floor (log n 2)) appending
		       (let ((m (expt 2 s)))
			 `((let ((w_m :type "const auto" :init (funcall "std::complex<float>"
									,(coerce (cos (/ (* pi -2) m)) 'single-float)
									,(coerce (sin (/ (* pi -2) m)) 'single-float))))
			     (for ((k 0) (< k N) (+= k ,m))
				  (let ((w :type "std::complex<float>" :ctor 1))
				    (dotimes (j ,(/ m 2))
				      (let ((t :ctor (* w (aref out (+ k j ,(/ m 2)))))
					    (u :ctor (aref out (+ k j)))
					    )
					(setf (aref out (+ k j)) (+ u t)
					      (aref out (+ k j ,(/ m 2))) (- u t)
					      w (* w w_m)))))))))
		       ))

	       
	       (function (android_main ((app :type android_app*))
				       void)
			 (funcall app_dummy)
			 (dotimes (i M_MAG_N)
			   (setf (aref m_fft_in i) 0.0
				 (aref m_fft_out_mag i) 0.0))
			 (let ((data :type userdata_t :ctor (list 0))
			       (sensor_manager :ctor (funcall ASensorManager_getInstance))
			       (looper :ctor (funcall ALooper_prepare ALOOPER_PREPARE_ALLOW_NON_CALLBACKS)))
			   (macroexpand (aassert (!= nullptr looper)))
			   (macroexpand (aassert (!= nullptr sensor_manager)))
			   (setf data.sensor_accelerometer (funcall ASensorManager_getDefaultSensor sensor_manager ASENSOR_TYPE_GYROSCOPE))
			  
			   (macroexpand (aassert (!= nullptr data.sensor_accelerometer)))
					;ASENSOR_TYPE_ACCELEROMETER
			   (setf data.sensor_accelerometer (funcall ASensorManager_getDefaultSensor sensor_manager ASENSOR_TYPE_GYROSCOPE))
			   
			   (macroexpand (aassert (!= nullptr data.sensor_accelerometer)))
			   (setf data.sensor_event_queue (funcall ASensorManager_createEventQueue sensor_manager looper LOOPER_ID_USER nullptr nullptr))
			   
			   (macroexpand (aassert (!= nullptr data.sensor_event_queue)))
			   (setf app->userData &data
				 app->onAppCmd handle_activity_lifecycle_events
				 app->onInputEvent handle_input_events)
			   (while (== 0 app->destroyRequested)
			     (let ((events :type int)
				   (source :type android_poll_source*)
				   (ident :type int :ctor (funcall ALooper_pollAll (? app->redrawNeeded 0 -1)
								   nullptr &events
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
					     (let ((mag :ctor (aref event.data 2) #+nil (funcall "std::sqrt" (+ (* event.acceleration.x  event.acceleration.x )
														(* event.acceleration.y  event.acceleration.y )
														(* event.acceleration.z  event.acceleration.z )))))
					       (setf (aref m_mag m_mag_idx) mag
					;(aref m_mag (% (+ 1 m_mag_idx) M_MAG_N)) 0.0
						     m_mag_idx (% (+ m_mag_idx 1)
								  M_MAG_N))
					       (if (== 0 (% m_mag_idx 8))
						   (statements
						    (dotimes (i M_MAG_N)
						      (setf (aref m_mag2 i) (aref m_mag i)))
						    (setf app->redrawNeeded 1)
						    ;;(macroexpand (alog (string "a: %f %f %f") (aref m_mag 0) (aref m_mag 1) (aref m_mag 2)))
						    
						    )
						   )
					       
					       (if (== 0 (% m_mag_idx 16))
						   (statements
						    (statements (dotimes (i M_MAG_N)
								  (setf (aref m_fft_in i) (aref m_mag i)))
								(macroexpand (benchmark (funcall fft m_fft_in m_fft_out)))
								(dotimes (i M_MAG_N)
								  (setf (aref m_fft_out_mag i) #+nil (funcall "std::abs" (aref m_fft_out i)) 
									(funcall "std::log" (+ 1 (funcall "std::abs" (aref m_fft_out i)) )
										 ))))
						     
						    #+nil(macroexpand (alog (string "fft finished")))
						    (setf app->redrawNeeded 1)
						    ))))))
				      (t
				       (statements
					(if (!= nullptr source)
					    (statements
					     (funcall source->process app source)))
					)))))
			       (if app->redrawNeeded
				   (statements
				    (funcall drawSomething app)
				    (setf app->redrawNeeded 0))))))))))
  (write-source "/home/martin/and/src/jni/hello" "cpp" code))

