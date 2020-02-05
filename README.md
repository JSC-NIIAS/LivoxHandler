# lidar_livox
> Service to handle Lidar Livox sensors data from models Mid-40, Mid-100, Horizon, Tele-15.

![](http://bb.niias/projects/ADO/repos/lidar_livox/browse/docs/lidars.png)

**lidar_livox** is the software designed for all Livox products. It is developed based on Qt C++ following Livox SDK Communication logic, and provides easy-to-use Qt style API. With **lidar_livox**, users can quickly connect to Livox products, receive point cloud data and visualize them.

![](http://bb.niias/projects/ADO/repos/lidar_livox/browse/docs/promo.png)

---

## Installation

>The installation procedures in Linux Ubuntu 16.04/14.04 32-bit LTS or Linux Mint 19.* 64-bit are shown here as examples.

### Dependencies

#### Qt:

Service requires Qt 5 [Qt 5.0.0+](https://www.qt.io/download-open-source#section-2). You need to visit Qt downloads page an download a 32-bit or 64-bit Linux installation depending your version of Ubuntu. The installation file can be also downloaded latest version through the command line using wget. Visit Qt install wiki for more info [Qt install wiki](https://wiki.qt.io/Install_Qt_5_on_Ubuntu).

For data visualization go to [lidar_livox.pro](http://bb.niias/projects/ADO/repos/lidar_livox/browse/lidar_livox.pro?at=refs%2Fheads%2Fdevelop) and change the flag from:
```
DEFINES -= WITH_GUI
```
to
```
DEFINES += WITH_GUI
```
and rebuild the project.

***Warning! Data visualization mode requires Qt Creator version 5.10.0 or more.* **

![](http://bb.niias/projects/ADO/repos/lidar_livox/browse/docs/scan.png)

#### ZCM:

ZCM is a micro-framework for message-passing and data-marshalling, designed originally for robotics systems where high-bandwidth and low-latency are critical and the variance in compute platforms is large. Installation here [ZCM Repository](https://github.com/ZeroCM/zcm).

#### Submodules:

Livox SDK requires [CMake 3.0.0+](https://cmake.org/), [Apache Portable Runtime (APR) 1.61+](http://apr.apache.org/) and [Boost 1.54+](https://www.boost.org/) as dependencies. You need to install boost, cmake using apt:
```
sudo apt install cmake libboost-atomic-dev libboost-system-dev
```
And download the newest apr linux source file in [Apache Portable Runtime (APR) 1.61+](http://apr.apache.org/):
```
tar -zxvf apr-1.X.X.tar.gz
cd apr-1.X.X
./configure --prefix=/usr/local/apr --disable-lfs
make && sudo make install
```
#### Compile Livox SDK & ZCM Types
In the Livox SDK directory, run the following commands to compile the project:
```
git clone ssh://git@bb.niias:7999/ado/lidar_livox.git
cd lidar_livox
git checkout develop
./scripts/update_submodules.sh
cd livox_sdk/build
PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/apr/lib/pkgconfig/ && export PKG_CONFIG_PATH
cmake ..
make
sudo make install
```
As the PKG_CONFIG_PATH will only take effect on the current terminal, you will need to run `PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/apr/lib/pkgconfig/ && export PKG_CONFIG_PATH` on every new shell, unless you add this line to your .bashrc as an environment variable.

---

## Development setup

Describing how to install all development dependencies and how to run an automated test-suite of some kind. Potentially do this for multiple platforms.

```sh
cd lidar_livox
git checkout develop
./scripts/build_all.sh
./scripts/run.sh
```
Or using Qt Creator:

*Projects->Run->* and insert the code:
```
-c ../lidar_livox/cfg/lidar_livox.ini -p lidar_livox
```

Optional:

>```--vgit or -V or --version``` - print repository version (git hash).
*Example:* ```hash=abe5cf73f7371cb2551505e78561fbff4021b98d, branch=develop, revcount=36, date=2020-02-04 21:12:41 +0300, author-name=Dmitrii Leliuhin, author-email=dleliuhin@gmail.com, comptime=Feb  5 2020T11:31:00```

>```--print_conf``` - print config file and exit.
>```--save_conf``` - save default config as file & exit

### Config file setup

Edit the ```cfg/lidar_livox.ini```. You need to change the *broadcast* parameter in *receive* group according to your sensor broadcast code.

For setting up extrinsic sensor parameters got to *offset* group and change the values. 
Zcm channels parameters you can find in *zcm_send* group.
The group *main_params* is responsible for data freequency to ZCM.
Lidar params setting up in *lidar_params* group. If you want to filter lidar data from noise (rain/fog) set the
```
weather_suppress=true
```

### Timestamp Synchronization
For sensor time synchro visit [Livox SDK time synchro](https://github.com/Livox-SDK/Livox-SDK/wiki/Timestamp-Synchronization).

---

## Release History

* 0.0.1
    * Bare harness for sensor.
* 1.0.0
    * Add pushing data to ZCM.

---
    
## Contributing

1. Clone it (<ssh://git@bb.niias:7999/ado/lidar_livox.git>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git add . & git commit -m "Feature. Add some fooBar."`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request to `develop`

---

## Support

Reach out to me at one of the following places!

- Slack at <a href="http://jscniias.slack.com" target="_blank">`jscniias.slack.com/Dmitrii Leliuhin`</a>
- Telegram at <a href="http://https://telegram.org" target="_blank">`@DLeliuhin`</a>
- Email at dleliuhin@gmail.com
---
