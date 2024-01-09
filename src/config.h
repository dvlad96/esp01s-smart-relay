#ifndef CONFIG_H
#define CONFIG_H

/************************************************
 *  Includes
 ***********************************************/
#include <ESP8266WiFi.h>

/************************************************
 *  Defines / Macros
 ***********************************************/

/* Internet configuration */
#define ESP01S_STATIC_IP                    (IPAddress(192, 168, 0, 18))
#define ESP01S_GATEWAY_IP                   (IPAddress(192, 168, 0, 1))
#define ESP01S_SUBNET_MASK                  (IPAddress(255, 255, 255, 0))
#define ESP01S_DNS_IP                       (IPAddress(8, 8, 8, 8))

#define NETWORK_SSID                        ("SSID")
#define NETWORK_PASSWORD                    ("PASS")
#define MAX_WIFI_CONNECTION_ATTEMPTS        (60U)

#define HTTP_SERVER_PORT                    (80U)

#define CHECK_NETWORK_STATUS_TIME_IN_MS     (60U * 1000U)

/************************************************
 *  Typedef definition
 ***********************************************/

/************************************************
 *  Public function definition
 ***********************************************/

#endif /* CONFIG_H */
