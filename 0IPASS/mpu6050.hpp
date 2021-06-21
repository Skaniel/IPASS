
//          Copyright Daniel Hoegee 2020 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef V1OOPC_EXAMPLES_MPU6050_HPP
#define V1OOPC_EXAMPLES_MPU6050_HPP

#include <hwlib.hpp>
#include "Class_xyz.hpp"

//defines for the registers addresses
#define MPU_ADDRESS 0x68
#define PWR_MGMT_1 0x6B
#define ACCEL_CONFIG 0x1B
#define GYRO_CONFIG 0x1C
#define ACCEL_XOUT 0x3B

//Number of sensor data registers
#define NUMB_SENSOR_REGS 14

//defines for register data
#define FS_SEL 0x01
#define AFS_SEL 0x02
#define CLKSEL 0x02

//needed for coversion from radians to degrees (1 rad = 57.295779513 deg)
#define RAD_TO_DEG 57.295779513

const int16_t readings = 200;

namespace mpulibrary {
///\brief
///Library for the MPU-6050 gravitation module
///
///\details
///This library extracts raw data from the mpu-6050. Returns raw data, temperature and the angle from the x and y axis

    class mpu_6050 {
    private:
        //initialize all variables
        hwlib::i2c_bus &bus;
        xyz xyz_accel_raw{0, 0, 0};
        xyz xyz_gyro_raw{0, 0, 0};

        xyz xyz_gyro{0, 0, 0};
        xyz xyz_accel{0, 0, 0};

        xyz xyz_gyro_avg{0, 0, 0};
        xyz xyz_accel_avg{0, 0, 0};

        int16_t temp_raw = 0;
        int16_t temperature = 0;
        double angle = 0.0;


        /// \brief
        /// Reads sensor data
        /// \details
        ///This function reads the registers of the mpu where the sensor values are stored
        /// register 3B/ 49
        void read_regs();

        /// \brief
        /// Calculate average
        /// \details
        /// This function calculates an average over n readers
        void average();

        /// \brief
        /// Write to i2c bus
        /// \details
        /// This function writes data to the i2c bus
        void i2c_write(uint8_t reg, uint8_t data);

    public:
        /// \brief
        /// Constructor
        /// \details
        ///This is the constructor for the MPU6050
        explicit mpu_6050(hwlib::i2c_bus &bus) : bus(bus) {}

        /// \brief
        /// Initialize MPU6050
        /// \details
        ///This function gets the MPU out of sleep mode and sets the accelerometer sensitivity
        void init();

        /// \brief
        /// Update angle and average
        /// \details
        /// This function calls to update the average and the angle
        void update();

        /// \brief
        /// Calculate and return angle
        /// \details
        /// This function calculates and returns the angle
        double get_angle() ;

        /// \brief
        /// Return temperature
        /// \details
        /// This function returns the temperature of the onboard sensor
        int16_t get_Temp() const {
            return temperature;
        }

        /// \brief
        /// Return X axis accelerometer
        /// \details
        /// This function returns the average of X axis accelerometer
        int16_t get_X_Accel() const {
            return xyz_accel_avg.x;
        }

        /// \brief
        /// Return Y axis accelerometer
        /// \details
        /// This function returns the average of Y axis accelerometer
        int16_t get_Y_Accel() const {
            return xyz_accel_avg.y;
        }

        /// \brief
        /// Return Z axis accelerometer
        /// \details
        /// This function returns the average of Z axis accelerometer
        int16_t get_Z_Accel() const {
            return xyz_accel_avg.z;
        }

        /// \brief
        /// Return X axis gyroscope
        /// \details
        /// This function returns the average of X axis gyroscope
        int16_t get_X_Gyro() const {
            return xyz_gyro_avg.x;
        }

        /// \brief
        /// Return Y axis gyroscope
        /// \details
        /// This function returns the average of Y axis gyroscope
        int16_t get_Y_Gyro() const {
            return xyz_gyro_avg.y;
        }

        /// \brief
        /// Return Z axis gyroscope
        /// \details
        /// This function returns the average of Z axis gyroscope
        int16_t get_Z_Gyro() const {
            return xyz_gyro_avg.z;
        }

        /// \brief
        /// NOT USED????
        /// \details
        ///
        int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }

    }; //Class
} //Namespace

#endif //V1OOPC_EXAMPLES_MPU6050_HPP