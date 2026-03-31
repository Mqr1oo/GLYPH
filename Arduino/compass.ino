//compass.ino
float offset_x = 0, offset_y = 0, offset_z = 0;
float scale_x = 1.0, scale_y = 1.0, scale_z = 1.0;
unsigned long os_calib_start = 0;
bool os_calib_started = false;
const unsigned long OS_CALIBRATION_DURATION = 60000; 


unsigned long last_compass_ms = 0;
float mag_field_strength = 0;


void initCompass() {
    offset_x = prefs.getFloat("mag_off_x", 0.0);
    offset_y = prefs.getFloat("mag_off_y", 0.0);
    offset_z = prefs.getFloat("mag_off_z", 0.0);
    
    scale_x = prefs.getFloat("mag_scale_x", 1.0);
    scale_y = prefs.getFloat("mag_scale_y", 1.0);
    scale_z = prefs.getFloat("mag_scale_z", 1.0);

    if (offset_x != 0.0 || offset_y != 0.0 || offset_z != 0.0) {
        calibDone = true;
        calibProgress = 100;
    } else {
        calibDone = false;
        calibProgress = 0;
    }
    last_compass_ms = millis();
}

/**
 * Proceseaza datele de la Magnetometru, Accelerometru si Giroscop
 * Special optimizat pentru prezenta unei baterii 18650 langa senzor
 */
void processCompassData() {
    if (!mag_ok || !imu_ok) return;

    sensors_event_t mag_event, accel, gyro, temp;
    mag.getEvent(&mag_event);
    imu.getEvent(&accel, &gyro, &temp);

    unsigned long now = millis();
    float dt = (now - last_compass_ms) / 1000.0;
    last_compass_ms = now;

    if (dt <= 0 || dt > 0.5) dt = 0.01; 

    float raw_x = mag_event.magnetic.x;
    float raw_y = mag_event.magnetic.y;
    float raw_z = mag_event.magnetic.z;
    if (!calibDone && currentState == PAGE_INIT_COMPASS) {
        static float smooth_x = 0, smooth_y = 0, smooth_z = 0;
        if (!os_calib_started) {
            os_calib_start = millis();
            os_calib_started = true;
            mag_min_x = 9999.0; mag_max_x = -9999.0;
            mag_min_y = 9999.0; mag_max_y = -9999.0;
            mag_min_z = 9999.0; mag_max_z = -9999.0;
            smooth_x = raw_x; smooth_y = raw_y; smooth_z = raw_z;
        }

        smooth_x = (smooth_x * 0.7) + (raw_x * 0.3);
        smooth_y = (smooth_y * 0.7) + (raw_y * 0.3);
        smooth_z = (smooth_z * 0.7) + (raw_z * 0.3);

        if (smooth_x < mag_min_x) mag_min_x = smooth_x;
        if (smooth_x > mag_max_x) mag_max_x = smooth_x;
        if (smooth_y < mag_min_y) mag_min_y = smooth_y;
        if (smooth_y > mag_max_y) mag_max_y = smooth_y;
        if (smooth_z < mag_min_z) mag_min_z = smooth_z;
        if (smooth_z > mag_max_z) mag_max_z = smooth_z;

        calibProgress = ((millis() - os_calib_start) * 100) / OS_CALIBRATION_DURATION;
        if (calibProgress >= 100) {
            calibDone = true; 
            os_calib_started = false;
            
            offset_x = (mag_max_x + mag_min_x) / 2.0;
            offset_y = (mag_max_y + mag_min_y) / 2.0;
            offset_z = (mag_max_z + mag_min_z) / 2.0;
            
            float dx = (mag_max_x - mag_min_x) / 2.0;
            float dy = (mag_max_y - mag_min_y) / 2.0;
            float dz = (mag_max_z - mag_min_z) / 2.0;
            float avg_d = (dx + dy + dz) / 3.0;
            
            scale_x = avg_d / dx;
            scale_y = avg_d / dy;
            scale_z = avg_d / dz;
            
            prefs.putFloat("mag_off_x", offset_x);
            prefs.putFloat("mag_off_y", offset_y);
            prefs.putFloat("mag_off_z", offset_z);
            prefs.putFloat("mag_scale_x", scale_x);
            prefs.putFloat("mag_scale_y", scale_y);
            prefs.putFloat("mag_scale_z", scale_z);
        }
        return; 
    }

    float m_x = (raw_x - offset_x) * scale_x;
    float m_y = (raw_y - offset_y) * scale_y;
    float m_z = (raw_z - offset_z) * scale_z;

    mag_field_strength = sqrt(m_x*m_x + m_y*m_y + m_z*m_z);

    static float f_ax = 0, f_ay = 0, f_az = 0;
    f_ax = (f_ax * 0.95) + (accel.acceleration.x * 0.05);
    f_ay = (f_ay * 0.95) + (accel.acceleration.y * 0.05);
    f_az = (f_az * 0.95) + (accel.acceleration.z * 0.05);
    float Ex = m_y * f_az - m_z * f_ay;
    float Ey = m_z * f_ax - m_x * f_az;
    float Ez = m_x * f_ay - m_y * f_ax;
    float normE = sqrt(Ex*Ex + Ey*Ey + Ez*Ez);
    if (normE > 0) { Ex /= normE; Ey /= normE; Ez /= normE; }

    float Nx = f_ay * Ez - f_az * Ey;
    float Ny = f_az * Ex - f_ax * Ez;
    float Nz = f_ax * Ey - f_ay * Ex;
    float normN = sqrt(Nx*Nx + Ny*Ny + Nz*Nz);
    if (normN > 0) { Nx /= normN; Ny /= normN; Nz /= normN; }
    float tilt_strength = abs(f_ax) / 9.81; 
    tilt_strength = constrain(tilt_strength, 0.0, 1.0);

    float absolute_heading = atan2(Ex, Nx) * 180.0 / PI + MAGNETIC_DECLINATION;
    float gyro_rate = (gyro.gyro.z * (1.0 - tilt_strength)) + (gyro.gyro.x * tilt_strength);
    gyro_rate *= (180.0 / PI); 

    float predicted_heading = currentHeading + (gyro_rate * dt);

    float diff = absolute_heading - predicted_heading;
    while (diff < -180.0) diff += 360.0;
    while (diff > 180.0) diff -= 360.0;
    currentHeading = predicted_heading + (diff * 0.015);

 
    while (currentHeading < 0) currentHeading += 360.0;
    while (currentHeading >= 360.0) currentHeading -= 360.0;
}