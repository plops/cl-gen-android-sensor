(eval-when (:compile-toplevel :execute :load-toplevel)
  (ql:quickload :cl-cpp-generator))

(in-package :cl-cpp-generator)

(defmacro e (&body body)
  `(statements (<< "std::cout" ,@(loop for e in body collect
				      (cond ((stringp e) `(string ,e))
					    (t e))) "std::endl")))
;; alternatives to serialize the sensor data for network transfer:
;; plain text
;; google protobuf
;; facebook beringei
;; thrift
;; https://capnproto.org/
;; flatbuffers
;; https://kentonv.github.io/capnproto/news/2014-06-17-capnproto-flatbuffers-sbe.html

;; read multiple sensors:
;; https://stackoverflow.com/questions/8989686/access-faster-polling-accelerometer-via-nativeactivity-ndk/10494475

(defun replace-all (string part replacement &key (test #'char=))
"Returns a new string in which all the occurences of the part 
is replaced with replacement."
    (with-output-to-string (out)
      (loop with part-length = (length part)
            for old-pos = 0 then (+ pos part-length)
            for pos = (search part string
                              :start2 old-pos
                              :test test)
            do (write-string string out
                             :start old-pos
                             :end (or pos (length string)))
            when pos do (write-string replacement out)
            while pos))) 


(defparameter *ndk-facts*
    `((10 "Android Studio 3 canary 3 does not support advanced profiling of native apps.")
      (20 "The system doesn't automatically disable sensors when the screen is off.")
      (30 "What is direct report mode")))
;; profiling https://developer.oculus.com/documentation/mobilesdk/latest/concepts/mobile-ndk-profiler/
;; gyro docs https://crypto.stanford.edu/gyrophone/files/gyromic.pdf
;; /home/martin/and/android-ndk-r14b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm apk/lib/armeabi-v7a/libhello.so
;; export PATH=$PATH:/data/data/com.termux/files/usr/bin ; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/data/com.termux/files/usr/lib 
(defmacro benchmark (&body body)
  `(let ((start :ctor (funcall current_time)))
     ,@body
     
     (macroexpand (alog (string "time: %f ms") (* (/ 1.0 1000.0) (- (funcall current_time) start))))
     ))

(defmacro atrace (name &body body)
  `(statements
       (funcall ATrace_beginSection ,name)
     ,@body
     (funcall ATrace_endSection)
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
		 (string ,(replace-all (format nil "assertion ~s failed" cond)
				       "
"
				       "\\
"))))))
(defun rev (x nn)
  (let ((n (floor (log nn 2)))
      (res 0))
  (dotimes (i n)
    (setf (ldb (byte 1 i) res) (ldb (byte 1 (- n 1 i)) x)))
  res))



(let* ((n 1024)
       (sensors '(accelerometer
		  magnetic_field
		  gyroscope))
       (code `(with-compilation-unit
		  
		  (include "android_native_app_glue.h")
		(include <android/log.h>)
		;(include <hardware/sensors.h>)
		;; (include <jni.h>)
		;; (include <android/window.h>)
		;; (include <android/rect.h>)
		;; (include <android/native_window_jni.h>)
		;; (include <android/native_window.h>)
		(include <android/sensor.h>)
		#+fft (include <cmath>)
		(include <algorithm>)
		(include <array>)
		(include <stdio.h>)
		#+fft (include <complex>)
		(include <sys/time.h>) ;; gettimeofday
					;(include <android/trace.h>) ;; >= api 23
		;;(include <inttypes.h>)
		(with-compilation-unit
		    (enum Constants (M_MAG_N ,n))

		  
		  #+fft
		  (decl (
			 
			 (m_fft_in :type "std::array<std::complex<float>,M_MAG_N>"  :init (list (list ,@ (loop for i below n collect 0.0))))
			 (m_fft_out :type "std::array<std::complex<float>,M_MAG_N>"  :init (list (list ,@ (loop for i below n collect 0.0))))
			 (m_fft_out_mag :type "std::array<float,M_MAG_N>" :init (list (list ,@ (loop for i below n collect 0.0))))
			 ))
		  (decl ((m_previous_x :type int :ctor -1)
			 ((aref m_mag M_MAG_N) :type float :init (list ,@ (loop for i below n collect 0.0)))
			 (m_mag_idx :type int :init 0)
			 ((aref m_mag2 M_MAG_N) :type float :init (list ,@ (loop for i below n collect 0.0)))
			 )))
		#+nil
		(function (current_time () "static inline uint64_t")
			  
			  (let ((tv :type "struct timeval"))
			    (funcall gettimeofday &tv nullptr)
			    (return (+ (* tv.tv_sec 1000000)
				       tv.tv_usec))))

		(struct userdata_t ()
			(decl ((move_x :type int32_t)
			       ,@(loop for e in sensors collect
				      `(,(format nil "sensor_~a" e) :type "const ASensor*"))
			       (sensor_event_queue :type ASensorEventQueue*))))
		
		(function (handle_input_events ((app :type android_app*)
						(event :type AInputEvent*)) int32_t)
			  
			  (let ((etype :ctor (funcall AInputEvent_getType event)))
			    
			    (case etype
			      (AINPUT_EVENT_TYPE_MOTION
			       (let ((action :ctor (funcall AMotionEvent_getAction event))
				     (pointer_index :type int32_t :ctor (>> (& action AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
									    AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT))
				     (pos_x :type int32_t :ctor (funcall AMotionEvent_getY event pointer_index)))
				 (raw "// FIXME why conversion from float to pos_x here?")
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
				  #+fft
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
			       ,@(loop for e in sensors collect
				       (let ((sensor (format nil "data->sensor_~a" e)))
					`(if (!= nullptr ,sensor)
					     (statements
					      
					      (funcall ASensorEventQueue_enableSensor data->sensor_event_queue ,sensor)
					      (let ((min_delay :ctor (funcall ASensor_getMinDelay ,sensor)))
					       (funcall __android_log_print ANDROID_LOG_INFO
							(string "native-activity")
							(string ,(format nil "~a_min_delay = %d" e))
							min_delay)
					       (raw "// setting rate only after sensor is enabled")
					       (funcall ASensorEventQueue_setEventRate data->sensor_event_queue ,sensor
							min_delay #+nil (* (/ (raw "1000L") 5000 ; 60
									      )					       1000)))))))))
			    (APP_CMD_LOST_FOCUS
			     (let ((data :ctor (funcall reinterpret_cast<userdata_t*> app->userData)))
			       ,@(loop for e in sensors collect
				       (let ((sensor (format nil "data->sensor_~a" e)))
					`(if (!= nullptr ,sensor)
					     (statements
					      (funcall ASensorEventQueue_disableSensor data->sensor_event_queue ,sensor
						       )))))))))
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
		#+fft
		(function  (bit_reverse_copy ((in :type "const std::array<std::complex<float>, N > &")
					      (out :type "std::array<std::complex<float>, N > &"))
					     "template<std::size_t N > void")
			   (setf ,@(loop for i below n appending
					`((aref out ,(rev i n)) (aref in ,i)))))
		#+fft
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
				   (let ((w :type "std::complex<float>" :ctor 1.0))
				     (dotimes (j ,(/ m 2))
				       (let ((t :ctor (* w (aref out (+ k j ,(/ m 2)))))
					     (u :ctor (aref out (+ k j)))
					     )
					 (setf (aref out (+ k j)) (+ u t)
					       (aref out (+ k j ,(/ m 2))) (- u t)
					       w (* w w_m)))))))))
			))

		
		
		(with-compilation-unit
		    ;; https://stackoverflow.com/questions/6033581/using-socket-in-android-ndk
		    ;; https://github.com/wzbo/Android-NDK-Socket/blob/master/sample/app/src/main/jni/tcomm.c
		    ;; https://github.com/mcxiaoke/android-ndk-notes/blob/master/posix-tcp-socket/src/main/jni/com_example_hellojni_Native.h
		    ;; github.com/plops/c_net

		    
		    (include <sys/socket.h>)
		  (include <arpa/inet.h>) ;; sockaddr_in 
		  (include <errno.h>)
		  (include <netinet/in.h>)
		  (include <unistd.h>)
		  
		  (class net_t ()
			 (access-specifier :private)
			 (decl ((m_fd :type int)
				(m_conn_fd :type int)
				(m_addr :type sockaddr_in)))
			 (access-specifier :public)
			 (function (net_t ((port :type int :default 1234)) nil)
				   (setf m_fd (funcall socket AF_INET SOCK_STREAM IPPROTO_TCP)
					 m_addr.sin_family AF_INET
					 m_addr.sin_port (funcall htons port)
					 m_addr.sin_addr.s_addr (funcall htonl INADDR_ANY))
				   (macroexpand (aassert (< 0 (funcall bind m_fd (funcall reinterpret_cast<sockaddr*> &m_addr)
								       (funcall sizeof m_addr)))))
				   (macroexpand (aassert (< 0 (funcall listen m_fd 10)))))
			 (function (~net_t () nil)
				   (funcall close))
			 
			 (function (accept () void)
				   (let ((len :type socklen_t :ctor (funcall sizeof m_addr)))
				     (setf m_conn_fd (funcall "::accept" m_fd (funcall reinterpret_cast<sockaddr*> &m_addr) &len))
				     (macroexpand (aassert (< 0 m_conn_fd)))))
			 (function (close () void)
				   (funcall "::close" m_fd))
			 (function (send ((data :type "std::array<char,N>")) "template<std::size_t N> int")
				   (let ((bytes_sent :ctor 0)
					 (bytes_left :ctor N))
				     (while (< 0 bytes_left)
				       (let ((bytes :ctor (funcall "::send" m_conn_fd (ref (aref data
												 bytes_sent))
								   bytes_left 0)))
					 (if (< bytes 0)
					     (statements
					      ,(let ((err-msgs
						      '((EAGAIN "The socket's  file  descriptor  is  marked  O_NONBLOCK  and  the requested operation would block.")
							(EBADF "A connection was forcibly closed by peer.")
							(ECONNRESET "A connection was forcibly closed by a peer.")
							(EDESTADDRREQ "The socket is not connection-mode and no peer address is set.")
							(EINTR  "A signal interrupted send() before any data was transmitted.")
							(EMSGSIZE "The  message  is too large to be sent all at once, as the socket requires.")
							(ENOTCONN "The socket is not connected.")
							(ENOTSOCK "The socket argument does not refer to a socket.")
							(EOPNOTSUPP "The socket argument is associated with a socket  that  does  not support one or more of the values set in flags.")
							(EPIPE  "The  socket  is  shut down for writing, or the socket is connection-mode and is no longer connected. In the latter case, and if the  socket  is  of  type  SOCK_STREAM or SOCK_SEQPACKET and the MSG_NOSIGNAL flag is not set, the SIGPIPE signal is generated to the calling thread.")
							(EACCES "The calling process does not have appropriate privileges.")
							(EIO    "An  I/O error occurred while reading from or writing to the file system.")
							(ENETDOWN "The local network interface used to  reach  the  destination  is down.")
							(ENETUNREACH "No route to the network is present.")
							(ENOBUFS "Insufficient  resources  were available in the system to perform the operation."))))
						 `(let (((aref err_msgs ,(length err-msgs)) :type "const char*"
							 :init (list ,@(loop for (e f) in err-msgs
									  collect `(string ,f))))
				 			(err_msg_lut
							 :ctor (lambda (((idx :type ssize_t)) :captures ("&") :ret ->int)
								 (case idx
								   ,@(loop for (e f) in err-msgs and i from 0 appending
									  `((,e (return ,i))))))))
						  (macroexpand (alog (string "net::send errno=%d") errno (aref err_msgs (funcall err_msg_lut errno))))))))
					 (-= bytes_left bytes)
					 (+= bytes_sent bytes)))
				     (return bytes_sent))))
		  #+nil (function (net_thread ((arg :type void*)) void*)
				  (let ((net :type net_t)
					(number :type int* :ctor (funcall static_cast<int*> arg)))
				    (macroexpand (alog (string "net_thread %d started")  *number))
				    (funcall net.accept)
				    (macroexpand (alog (string "net_thread %d accepted connection")  *number))
				    ,(let ((m "hello"))
				       `(let ((msg :type ,(format nil "std::array<unsigned char,~a>" (+ 1 (length m))) :ctor (list (string ,m))))
					  (funcall net.send msg)))
				    (macroexpand (alog (string "net_thread %d finished")  *number)))
				  (return nullptr)))
		#+nil (with-compilation-unit
			  (include <pthread.h>)
			(include <semaphore.h>))

		


		;; compute the message length by emulating the snprintf format string with common lisp format function
		;; remember to add 1 char for the trailing zero
		,(let* ((msg-proto (format nil "~3a ~12a ~12a ~12a ~12a~%" #+nil "~3a %12lld %+12.5f %+12.5f %+12.5f\\n"
					   'typ 'time 'x 'y 'z))
			(msg-n (+ 3 (length msg-proto)) ; (+ 4 12 1 12 1 12 1 12  1)
			 )
			(msg-type (format nil "std::array<char,~a>" msg-n)))
		   (defparameter *msg-proto* msg-proto)
		   `(with-compilation-unit
			(with-compilation-unit
		    ;; https://codereview.stackexchange.com/questions/84109/a-multi-threaded-producer-consumer-with-c11
		    ;; https://baptiste-wicht.com/posts/2012/04/c11-concurrency-tutorial-advanced-locking-and-condition-variables.html
		    (include <thread>)
		  (include <deque>)
		  (include <condition_variable>)
		  
		  (class mt_buffer_t ()
			 (access-specifier :private)
			 (decl ((m_mutex :type "std::mutex")
				(m_cond :type "std::condition_variable")
				(m_buffer :type ,(format nil "std::deque<~a>" msg-type))
				(m_size :type "const unsigned int" :init 10)))
			 (access-specifier :public)
			 (function (add ((num :type ,(format nil "const ~a&" msg-type))) void)
				   (while true
				     (let ((locker :type "std::unique_lock<std::mutex>" :ctor m_mutex))
				       (funcall m_cond.wait
						locker
						(lambda (() :captures (this))
						  (return (< (funcall m_buffer.size) m_size))))
				       (funcall m_buffer.push_back num)
				       (funcall locker.unlock)
				       (funcall m_cond.notify_all)
				       (return))))
			 (function (remove () ,msg-type)
				   (while true
				     (let ((locker :type "std::unique_lock<std::mutex>" :ctor m_mutex))
				       (funcall m_cond.wait
						locker
						(lambda (() :captures (this))
						  (return (< 0 (funcall m_buffer.size)))))
				       (let ((back :ctor (funcall m_buffer.back)))
					 (funcall m_buffer.pop_back)
					 
					 (funcall locker.unlock)
					 (funcall m_cond.notify_all)
					 (return back))))))

		  (function (mt_consumer ((buffer :type "mt_buffer_t&")) void)
			    (let ((net :type net_t))
			      (macroexpand (alog (string "mt_consumer thread started")))
			      (funcall net.accept)
			      (macroexpand (alog (string "mt_consumer accepted connection")))
			      (while true
				#+nil (macroexpand (alog (string "mt_consumer waits for value")))
				(let ((value :ctor (funcall buffer.remove)))
				  (statements
				    (funcall net.send value))))
			      (macroexpand (alog (string "mt_consumer finished")))))
		  (function (mt_produce ((buffer :type mt_buffer_t&) (value :type ,(format nil "const ~a&" msg-type))) void)
			    (funcall buffer.add value)))

		      (function (android_main ((app :type android_app*))
					     void)
			       (funcall app_dummy)
			       #+fft
			       (dotimes (i M_MAG_N)
				 (setf (aref m_fft_in i) 0.0
				       (aref m_fft_out_mag i) 0.0))

			       #+nil (let ((thread_1 :type pthread_t)
					   (thread_num_1 :type int :ctor 1)
					   (ret :ctor (funcall pthread_create &thread_1 nullptr net_thread (funcall static_cast<void*> &thread_num_1)))))
			  
			       (let ((mt_buffer :type "mt_buffer_t")
				     (consumer1 :type "std::thread" :ctor (comma-list mt_consumer (funcall "std::ref" mt_buffer))))
			    
			    
			    
				 (let ((data :type userdata_t :ctor (list 0))
				       (sensor_manager :ctor #+nil
						       (funcall ASensorManager_getInstanceForPackage nullptr)
						       (funcall ASensorManager_getInstance))
				       (looper :ctor (funcall ALooper_prepare ALOOPER_PREPARE_ALLOW_NON_CALLBACKS)))
				   (macroexpand (aassert (!= nullptr looper)))
				   (macroexpand (aassert (!= nullptr sensor_manager)))
				   ,@ (loop for e in sensors appending
					   (let ((var (format nil "data.sensor_~a" e)))
					     `((setf ,var (funcall ASensorManager_getDefaultSensor sensor_manager ,(string-upcase (format nil "ASENSOR_TYPE_~A" e))))
					       (if (!= nullptr ,var)
						   (statements
						    (funcall __android_log_print ANDROID_LOG_INFO
							     (string "native-activity")
							     (string ,(format nil "error in getDefaultSensor ~a" var))
							     )))
					       #+nil (macroexpand (aassert (!= nullptr ,var))))))
				   (setf  data.sensor_event_queue (funcall ASensorManager_createEventQueue sensor_manager looper LOOPER_ID_USER nullptr nullptr))
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
						     (case event.type
						       ,@(loop for (e f g h) in '((accelerometer event.acceleration.x event.acceleration.y event.acceleration.z)
										  (gyroscope (aref event.data 0) (aref event.data 1) (aref event.data 2))
										  (magnetic_field event.magnetic.x event.magnetic.y event.magnetic.z))
							    collect
							      `(,(string-upcase (format nil "ASENSOR_TYPE_~a" e))
								 (raw "// timestamp is in nanoseconds")
								 #+nil (let ((s :type "std::stringstream"))
									 (<< s
									     (string ,(subseq (format nil "~a " e) 0 3))
									     (funcall "std::setw" 12) event.timestamp (string " ")
									     (funcall "std::setw" 12) ,f (string " ")
									     (funcall "std::setw" 12) ,g (string " ")
									     (funcall "std::setw" 12) ,h)
									 (macroexpand (alog (funcall s.c_str))))

								 (let (
								       (s :type ,(format nil "static ~a" msg-type))
								        )
								   #+str-test
								   (let ((sb :type "static std::array<char,1024>"))
								     (funcall sprintf (funcall sb.data)
										    (string ,(format nil "~3a %12lld %+12.5f %+12.5f %+12.5f\\n"
												     (subseq (format nil "~a" e) 0 3)))
										    (funcall "static_cast<long long>" event.timestamp) ,f ,g ,h)
									       (macroexpand (alog (string "len %d") (funcall strlen (funcall sb.data)))))
								   (funcall snprintf (funcall s.data) ,msg-n (string ,(format nil "~3a %12lld %+12.5f %+12.5f %+12.5f\\n"
															  (subseq (format nil "~a" e) 0 3)))
									    (funcall "static_cast<long long>" event.timestamp) ,f ,g ,h)
								   (funcall mt_produce (funcall "std::ref" mt_buffer) s)
								
								   #+nil (macroexpand (alog (string "%s") (funcall s.data))))
								 #+nil
								 (macroexpand (alog 
									       (string ,(format nil "~3a %12lld %+12.5f %+12.5f %+12.5f" (subseq (format nil "~a" e) 0 3)))
									       (funcall "static_cast<long long>" event.timestamp) ,f ,g ,h))))
						       )
						     #+nil (statements ;let ((time :ctor (funcall get_time)))
							    (funcall __android_log_print ANDROID_LOG_INFO
								     (string "native-activity")
								     (string "accel %f")
					;time
								     event.acceleration.x))
						     (if (== event.type ASENSOR_TYPE_GYROSCOPE)
							 (let ((mag :ctor (aref event.data 2) #+nil (funcall "std::sqrt" (+ (* event.acceleration.x  event.acceleration.x )
															    (* event.acceleration.y  event.acceleration.y )
															    (* event.acceleration.z  event.acceleration.z )))))
							   (setf (aref m_mag m_mag_idx) mag
					;(aref m_mag (% (+ 1 m_mag_idx) M_MAG_N)) 0.0
								 m_mag_idx (% (+ m_mag_idx 1)
									      M_MAG_N))
							   #+nil (funcall mt_produce (funcall "std::ref" mt_buffer) (funcall static_cast<int> (* 1000 mag)))
							   #+nil (macroexpand (alog (string "mag: %f") mag))
							   (if (== 0 (% m_mag_idx 8))
							       (statements
								(dotimes (i M_MAG_N)
								  (setf (aref m_mag2 i) (aref m_mag i)))
								(setf app->redrawNeeded 1)
								#+nil(macroexpand (alog (string "a: %f %f %f") (aref m_mag 0) (aref m_mag 1) (aref m_mag 2)))
						       
								)
							       )
						  
							   #+fft (if (== 0 (% m_mag_idx 16))
								     (statements
								      (statements
							      
								       (dotimes (i M_MAG_N)
									 (setf (aref m_fft_in i) (aref m_mag i)))
								       (macroexpand (benchmark (funcall fft m_fft_in m_fft_out)))
								       (dotimes (i M_MAG_N)
									 (setf (aref m_fft_out_mag i) #+nil (funcall "std::abs" (aref m_fft_out i)) 
									       (funcall "std::log" (+ 1 (funcall "std::abs" (aref m_fft_out i)) )
											))))
							     
								      #+nil(macroexpand (alog (string "fft finished")))
								      (setf app->redrawNeeded 1)
								      )))))))
					      (t
					       (statements
						(if (!= nullptr source)
						    (statements
						     (funcall source->process app source)))
						)))))
				       (if app->redrawNeeded
					   (statements
					    (funcall drawSomething app)
					    (setf app->redrawNeeded 0)))))))))))))
  (write-source "/home/martin/and/src/jni/hello" "cpp" code))


