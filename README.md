# Industrial Machine State Monitoring System
Machine monitoring is the process of connecting your machine to the internet to collect and interpret data by using the potential of the cloud. Before  IoT, Investigating the status of any given machine required labour intensive manual intervention. The cost to find a problem, in terms of downtime, was high.  The scope of the Industrial Machine State Monitoring System (IMSMS) project is to retrofit existing machines and provide a way for data analysis

# Assumptions, Dependencies, Constraints
All the events will be generated in the Lab and no real interface will be available.

# Requirements
![alt text](https://raw.githubusercontent.com/srsaidas/Industrial-Machine-State-Monitoring-System/master/image/mag3.png)
## Station (STA)
The block diagram for a station which should be capable of the following feature
* Senses the temperature and humidity of the surrounding area. 
* Transmit temperate and humidity data to Gateway through WiFi
![alt text](https://raw.githubusercontent.com/srsaidas/Industrial-Machine-State-Monitoring-System/master/image/sta.png)
## Gateway
Every station is connected to gateway and gateway is connected to internet. It provides and Isolation between the Internet and the local systems. Gateway is capable of the following feature
* Collect data send by the station
* Process the data 
* Send necessary data to the cloud 

~Thanks

[Saidas S R!](https://www.linkedin.com/in/saidassr/)
