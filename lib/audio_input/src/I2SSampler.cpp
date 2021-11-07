
#include "I2SSampler.h"
#include "driver/i2s.h"

I2SSampler::I2SSampler(i2s_port_t i2sPort, const i2s_config_t &i2s_config) : m_i2sPort(i2sPort), m_i2s_config(i2s_config)
{
    i2s_driver_install(m_i2sPort, &m_i2s_config, 0, NULL); //It is logical to do here....
    //configureI2S(); //so as this... but this is TypeDependantd...
}

void I2SSampler::start()
{
    Serial.printf("Start I2S Sampler! \n");
    //install and start i2s driver
    i2s_driver_install(m_i2sPort, &m_i2s_config, 0, NULL);
    // set up the I2S configuration from the subclass
    Serial.println("I2S start() after driver install");
    //configureI2S();

    //i2s_start(m_i2sPort); //I hope this is logical???
}

void I2SSampler::stop()
{
    // stop the i2S driver
    i2s_driver_uninstall(m_i2sPort);

    //May be stop I2S but not the driver uninstall completely...
    //i2s_stop(m_i2sPort);
}
