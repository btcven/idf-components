#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TX_BUF_ENABLE 1  /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_ENABLE 1  /*!< I2C master doesn't need buffer */

#define I2C_MASTER_ACK_EN true   /*!< Enable ack check for master */
#define I2C_MASTER_ACK_DIS false /*!< Disable ack check for master */


#endif // CONSTANTS_H