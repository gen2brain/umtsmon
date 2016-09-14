UMTSmon
=======

Qt4 port of [UMTSmon](http://umtsmon.sourceforge.net).

UMTSmon is a tool to handle UMTS (3G) devices in Linux.

![screenshot][screenshot]


Prerequisites
--------------

You need to have Qt version 4 installed, including the development libs and tools. Usually these are packages called qt-devel and such.

You need to have the header files for libusb installed.  Some systems have this in a package called libusb-devel.

Make sure that you also have the related development tools installed: you should have a directory with 'uic', 'qmake' and 'moc' in your search path.
Alternatively, you might want to call qmake with the full path and it will adjust the Makefile to list full paths to the other binaries.


Install
-------

    qmake
    make clean all
    make install

The make install will install the umtsmon application by default in /usr/local/bin

[screenshot]: https://lh3.googleusercontent.com/KhODSKIgA6udPdd1O6doDs1TxsDzDKpNErev6KkjCvRZ5ke8WD5qVM0nEc0umh71dLeLdshBv-bXQn9Lrm-eeejmx77WJbI-VFKaFUh0ZGkv14gf5SE2L-vmKiOb4xyKpVuTomPkg3LjOaOwFCMv2T0GmvVoNkyHyPtX6fLUyXDZiurwRbom27Zl-i5Z27hH85F_1ZNxxKWAjo0eQz1iMZmwnjmqiQ0Pv1j6a0BLviDBug_EvKmqgENkZid4K14CpoAfmLf-_MxQExanNvBbnK308XRTO2zVxDp_Xtw8FoJyNczWoC737a0vNS3d5cIZeiVeTQFaRiJQ2jySaTjPTRmBIf1VkM5TxTW5iv4HZhG4lfIdb-jewFNsyl_cNnzIbjpbq7o1I0GclZ2C7TyihFjxxY6R20_k7mJ7bNesLC9Nau7nStpJ7epuOcPnaSvnDRMO1Ja-TSLd424aRnanLbeesRVi1t2860kZS0Z8BMCJVlm-ACyLG3-F9Cymv323zlQ4-aqram7KGmjeYxQji-rtuFanzCFtJ5CsRm_qRgNLacExTS4jaiyIxAveKTTxL07DpyKnmEgWbiUWXMQP1NhI5YjrPB3LN_Ga6VdXHDniceK9=w360-h248-no "Screenshot"
