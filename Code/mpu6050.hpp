
//          Copyright Daniel Hoegee 2020 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef V1OOPC_EXAMPLES_MPU6050_HPP
#define V1OOPC_EXAMPLES_MPU6050_HPP

//doxygen mainpage:
///\mainpage
///This library is made for interfacing with the mpu6050
///\image html mpu6050_img.jpg
/// \author Daniël Hoegee
/// \version 1 (last modified 30-06-2021)
/// \copyright Boost license


#include <hwlib.hpp>
#include "Class_xyz.hpp"

//for all registers see register map at:
//https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf

//defines for the registers addresses
#define MPU_ADDRESS 0x68
#define PWR_MGMT_1 0x6B
#define ACCEL_CONFIG 0x1B
#define GYRO_CONFIG 0x1C
#define ACCEL_XOUT 0x3B

//Number of sensor data registers
#define NUMB_SENSOR_REGS 14

//defines for register data
#define FS_SEL 0x08
#define AFS_SEL 0x10
#define CLKSEL 0x02

//needed for conversion from radians to degrees (1 rad = 57.295779513 deg)
#define RAD_TO_DEG 57.295779513

#define samples 7

///\brief
///Namespace for the MPU-6050 gravitation module
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
        xyz xyz_accel_raw;
        xyz xyz_gyro_raw;

        xyz xyz_gyro_avg;
        xyz xyz_accel_avg;

        //arrays for holding raw sensor values for all sensors (only using accelerometer data for angle calculation)
        int16_t smooth_accel_x[samples];
        int16_t smooth_accel_y[samples];
        int16_t smooth_accel_z[samples];

        int16_t smooth_gyro_x[samples];
        int16_t smooth_gyro_y[samples];
        int16_t smooth_gyro_z[samples];

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
        /// Calculates the angle
        /// \details
        /// This function calculates the angle from the 3 accelerometer axis
        void calculate_angle();

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
        mpu_6050(hwlib::i2c_bus &bus) : bus(bus) {}

        /// \brief
        /// Initialize MPU6050
        /// \details
        ///This function gets the MPU out of sleep mode and sets the accelerometer and gyroscope sensitivity
        void initialize();

        /// \brief
        /// Filters raw sensor data
        /// \details
        /// This function filters raw data by using a rolling array, new data is added to the back of the array.
        /// Then the data is sorted and the average is returned
        /// \warning
        /// This function needs a array for each sensor
        int16_t smoothData(int16_t rawData, int16_t *sensorArray);

        /// \brief
        /// Update angle and average from sensors
        /// \details
        /// This function calls to update the average of the sensor and to calculate the angle from the accelerometer
        void update();

        /// \brief
        /// Returns angle
        /// \details
        /// This function returns the angle
        [[nodiscard]] double get_angle() const {
            return angle;
        }

        /// \brief
        /// Return temperature
        /// \details
        /// This function returns the temperature of the onboard sensor
        [[nodiscard]] int16_t get_Temp() const {
            return temperature;
        }

        /// \brief
        /// Return X axis accelerometer
        /// \details
        /// This function returns the average of X axis accelerometer
        [[nodiscard]] int16_t get_X_Accel() const {
            return xyz_accel_avg.x;
        }

        /// \brief
        /// Return Y axis accelerometer
        /// \details
        /// This function returns the average of Y axis accelerometer
        [[nodiscard]] int16_t get_Y_Accel() const {
            return xyz_accel_avg.y;
        }

        /// \brief
        /// Return Z axis accelerometer
        /// \details
        /// This function returns the average of Z axis accelerometer
        [[nodiscard]] int16_t get_Z_Accel() const {
            return xyz_accel_avg.z;
        }

        /// \brief
        /// Return X axis gyroscope
        /// \details
        /// This function returns the average of X axis gyroscope
        [[nodiscard]] int16_t get_X_Gyro() const {
            return xyz_gyro_avg.x;
        }

        /// \brief
        /// Return Y axis gyroscope
        /// \details
        /// This function returns the average of Y axis gyroscope
        [[nodiscard]] int16_t get_Y_Gyro() const {
            return xyz_gyro_avg.y;
        }

        /// \brief
        /// Return Z axis gyroscope
        /// \details
        /// This function returns the average of Z axis gyroscope
        [[nodiscard]] int16_t get_Z_Gyro() const {
            return xyz_gyro_avg.z;
        }
    }; //Class
} //Namespace

#endif //V1OOPC_EXAMPLES_MPU6050_HPP