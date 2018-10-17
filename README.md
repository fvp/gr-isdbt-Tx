### ISDB-T transmitter in GNU Radio

**An open source implementation of a full seg transmitter for the Digital Television standard ISDB-T (ARIB's STD-B31) in GNU Radio.**

In particular, here in Uruguay we use the so-called "international" version, or ISDB-Tb (ABNT NBR 15601). However, the transmission scheme is exactly the same as in the original version, thus both references are equivalent.  

We are still in development stages, and any help and/or feedback is still welcome. Since ISDB-T is similar to DVB-T, some of our blocks are heavily based on Bogdan Diaconescu's (BogdanDIA on git) DVB-T implementation (see https://github.com/BogdanDIA/gr-dvbt). 

This transmitter is currently operational, and commercial TV sets suppporting ISDB-T standard sucessfully decodes all transmision layers (HD, SD and One-Seg). We are using Ettus Research's USRP B100 as an antenna, which is currently discontinued, but any antena from ettus is fully supported. For more information about hardware, please see https://www.ettus.com/product/category/USRP-Bus-Series.

Testing without antenna is also possible, but you need to use as a reciever the project developed by ARTES group, which you can find here: https://github.com/git-artes/gr-isdbt.

**Build instructions**

For a system wide installation:

    git clone https://github.com/jhernandezbaraibar/gr-isdbt-Tx.git 
    cd gr-isdbt-Tx  
    mkdir build  
    cd build  
    cmake ../  
    make && sudo make install  

For a user space installation, or GNU Radio installed in a location different from the default location /usr/local:

    git clone https://github.com/jhernandezbaraibar/gr-isdbt-Tx.git
    cd gr-isdbt-Tx  
    mkdir build  
    cd build  
    cmake -DCMAKE_INSTALL_PREFIX=<your_GNURadio_install_dir> ../ 
    make
    make install  

On Debian/Ubuntu based distributions, you may have to run:

    sudo ldconfig  
    

**Notes and examples**

The transmitter is now complete regarding operation, we are curreltly working in extra features and optimizing some of the blocks. This project is our final project for our Engineering Degree, and work is now focused on documentation. We will add more examples as soon as we get more time. Please contact us if you have any questions regarding the project, you will be answered asap.

For testing purposes, you may want to try this flowgraph:  https://mega.nz/#!THxkDKKA!iSUYHXiwHGvPrGbd8vwYZNN3VkrxHo4U4JWpVh19fmM

Source BTS for the test can be downloaded here:  https://mega.nz/#!uXJRlKZL!R0qNIK1cpk79DWIz1-2Vu9lqkTjW6q1DC1DMQGeQze4

**Contact Information**

You can contact the authors, gr-isdbt-tx was developed by:

Santiago Castro Guzman      santiago.castro@fing.edu.uy

Javier Hernandez Baraibar   javier.hernandez@fing.edu.uy
                            www.linkedin.com/in/javier-hernández-baráibar
                            

or you can also contact the project supervisors:


Dr. Ing. Federico Larroca   flarroca@fing.edu.uy

MsC. Ing. Pablo Flores      pablof@fing.edu.uy


IIE Instituto de Ingeniería Eléctrica  
Facultad de Ingeniería  
Universidad de la República  
Montevideo, Uruguay  
http://iie.fing.edu.uy/investigacion/grupos/artes/  
  
Please refer to the LICENSE file for contact information and further credits.   
