<!DOCTYPE TS><TS>
<context>
    <name>@default</name>
    <message>
        <source>not pending for any password.</source>
        <translation>venter ikke på passord.</translation>
    </message>
    <message>
        <source>waiting for the PIN code for the SIM card.</source>
        <translation>venter på PIN-koden til SIM-kortet.</translation>
    </message>
    <message>
        <source>waiting for the PUK code to unblock the SIM card again.</source>
        <translation>venter på PUK-koden for å låse opp SIM-kortet.</translation>
    </message>
    <message>
        <source>waiting phone-to-SIM card password to be given.</source>
        <translation>venter på telefon-til-SIM-kort passord.</translation>
    </message>
    <message>
        <source>waiting phone-to-very SIM card password to be given.</source>
        <translation>venter på telefon-til-SIM-kort passord.</translation>
    </message>
    <message>
        <source>waiting phone-to-very first SIM card password to be given.</source>
        <translation>venter på første telefon-til-SIM-kort passord.</translation>
    </message>
    <message>
        <source>waiting phone-to-very first SIM card unblocking password to be given.</source>
        <translation>venter på første telefon-til-SIM-kort passord for å låse opp.</translation>
    </message>
    <message>
        <source>waiting SIM PIN2 to be given.</source>
        <translation>venter på PIN2-koden til SIM-kortet.</translation>
    </message>
    <message>
        <source>waiting SIM PUK2 to be given.</source>
        <translation>venter på PUK2-koden til SIM-kortet.</translation>
    </message>
    <message>
        <source>waiting network personalisation password to be given.</source>
        <translation>venter på personlig nettverkspassord.</translation>
    </message>
    <message>
        <source>waiting network personalisation unblocking password to be given.</source>
        <translation>venter på personlig nettverkspassord for å låse opp.</translation>
    </message>
    <message>
        <source>waiting network subset personalisation password to be given.</source>
        <translation>venter på personlig subset nettverkspassord.</translation>
    </message>
    <message>
        <source>waiting network subset personalisation unblock code to be given.</source>
        <translation>venter på personlig subset nettverkspassord for å låse opp.</translation>
    </message>
    <message>
        <source>unknown (PH-SP PIN) - please contact author</source>
        <translation>ukjent (PH-SP PIN) -   ta kontakt med programforfatter</translation>
    </message>
    <message>
        <source>unknown (PH-SP PUK) - please contact author</source>
        <translation>ukjent (PH-SP PUK) -   ta kontakt med programforfatter</translation>
    </message>
    <message>
        <source>unknown (PH-CORP PIN) - please contact author</source>
        <translation>ukjent (PH-CORP PIN) -   ta kontakt med programforfatter</translation>
    </message>
    <message>
        <source>unknown (PH-CORP PUK) - please contact author</source>
        <translation>ukjent (PH-CORP PUK) -   ta kontakt med programforfatter</translation>
    </message>
</context>
<context encoding="UTF-8">
    <name>AboutDialogUmtsmon</name>
    <message>
        <source>About</source>
        <translation type="obsolete">Om</translation>
    </message>
    <message>
        <source>&lt;b&gt;&lt;font size=&quot;+2&quot;&gt;UMTSmon&lt;/font&gt; - Monitoring your UMTS PCMCIA card&lt;/b&gt;&lt;br&gt;&lt;br&gt;
(C) 2006,2007,2008 by Klaas van Gend and others.&lt;br&gt;&lt;br&gt;

&lt;b&gt;This application is licensed by the GPL v2 license.&lt;/b&gt;&lt;br&gt;
The COPYING tab explains the rights and constraints that are granted to you due to this software license.
&lt;br&gt;
Refer to the AUTHORS tab to find out what parts of this application were derived/taken from other sources.</source>
        <translation type="obsolete">&lt;b&gt;&lt;font size=&quot;+2&quot;&gt;UMTSmon&lt;/font&gt; - Verktøy for å bruke UMTS PCMCIA kort&lt;/b&gt;&lt;br&gt;&lt;br&gt;
(C) 2006,2007,2008 Klaas van Gend med flere.&lt;br&gt;&lt;br&gt;(new line)

&lt;b&gt;Dette programet er lisensiert under  GPL v2.&lt;/b&gt;&lt;br&gt;
Arkfanen merket Lisens forklarer hvilke rettigheter og begrensninger du har med denne lisensen.
&lt;br&gt;
Se på arkfanen Forfattere for å se hvilke deler av umtsmon som er hentet inn fra andre program-.</translation>
    </message>
    <message>
        <source>version updated at runtime</source>
        <translation type="obsolete">versjon oppdatert ved start</translation>
    </message>
    <message>
        <source>COPYING</source>
        <translation type="obsolete">Lisens</translation>
    </message>
    <message>
        <source>AUTHORS</source>
        <translation type="obsolete">Forfattere</translation>
    </message>
    <message encoding="UTF-8">
        <source>
AUTHORS OF UMTSMON
==================

umtsmon was written by Klaas van Gend and Christofer Wesseling in 2006, 2007 and 2008.
It is licensed under the GPLv2 License, see the file COPYING

However, Klaas is not the only copyright holder as some parts were derived
from or based on other sources. Also: several people gave advise on some 
implementation topics. This file tries to list those contributions:



1)
the SerialPort class was based on COMGT 0.3 by Paul Hardwick
COMGT itself was based a package called dcon by Daniel Chouinard
See http://www.pharscape.org/content/view/46/70/
COMGT v0.3 was licensed under the GPLv2 or later.

Paul Hardwick also contributed to the DetectCard and Device classes.


2)
Debugging some issues in the SerialPort class was done with suggestions made by:
* Roel van den Broeck
* Martijn Wijms
* Tom Geelen
* Wouter Moors

3)
The image ok.png is taken from the kdelibs3 RPM from SuSE9.3
It is from the CrystalSVG icon set. According to the RPM, all files are 
licensed through the GPL or LGPL.

4)
The UMTSmon logo was drawn by &apos;Tazz_tux&apos;.

5)
Autodetection for the Nozomi 2.2+ drivers and multiple pcmcia socket enumeration
was contributed by Stefan Seyfried [seife@suse.de]
Stefan also worked on several other issues related to card detection.

6)
The &apos;Runner&apos; code to run other programs from umtsmon, specifically the piece to communicate
through pipes with the program run, was inspired by code from the following web page:
http://www.cryptnet.net/fdp/gtk/exec.html
the code on that page is GPL and copyrighted by V. Alex Brennen [vab@metanet.org] .

7)
Icons in the images/{16|22|SVG} directories come from the Tango icon
set, available at: http://tango.freedesktop.org/Tango_Icon_Library
These icons are licensed under the &quot;Creative Commons Attribution Share-Alike license&quot;
We think that we comply with the requirements of the license, even though we ship
the icons inside the umtsmon binary.

8)
Several compile-related and user interface issues were fixed by Gerald Pfeifer.

9)
fixup old/original Vodafone 3G detection, patch by dj9pz (Arnd Gronenberg)

10)
Huawei E220 fixes by Norbert Preining (norbusan)

11)
Lubomir Schmidt started translation into German and did lots of testing.

12)
additional translation into German by Philipp Kießler and Dietmar Maurer.

13)
translation work into Portugese (pt_PT) by Pedro Algarvio.

14)
some fixes by Carsten Clasohm.

15)
several UI improvements by Danny Kukawka. 



If you have found any ommissions, please contact the author through the Sourceforge website:
http://www.sourceforge.net/projects/umtsmon
                            </source>
        <translation type="obsolete">
Forfattere av  UMTSMON
==================
umtsmon er skrevet av  Klaas van Gend og Christofer Wesseling i 2006, 2007 og 2008.
Umtsmon er skrevet med lisens  GPLv2 , se arkfanen Lisens

Klaas er ikke den eneste som har copyright, enkelte deler av umts er avledet
eller basert på andre kilder. Flere har også kommet med konstruktive 
tilbakemeldinger. Denne fila forsøker å liste opp de fleste bidragsytere:


1)
SerialPort klassen er basert på  COMGT 0.3 av Paul Hardwick
COMGT er igjen vasert på programmet dcon av Daniel Chouinard
Se http://www.pharscape.org/content/view/46/70/
COMGT v0.3 er lisensiert med GPLv2 eller senere.

Paul Hardwick har også bidratt til DetectCard og Device klassene.

2)
Feilsøking i SerialPort  klasse ble utført sammen med:
* Roel van den Broeck
* Martijn Wijms
* Tom Geelen
* Wouter Moors

3)
Bildet ok.png er tatt fra kdelibs3 RPM fra SuSE9.3
Bildet er en del av CrystalSVG ikonpakken. I følge RPM pakken, så er 
alle filene i ikonpannen lisesniser med  GPL eller LGPL.

4)
UMTSmon logo ble tegnet av &apos;Tazz_tux&apos;.

5)
Autodeteksjonen for Nozomi 2.2+ driverene og multiple pcmcia kort
er skrevet av Stefan Seyfried [seife@suse.de]
Stefan har også bidratt generelt til koden for autodeteksjon av kort.

6)
&apos;Runner&apos; koden som gjør det mulig å kjøre andre program direkte fra umtsom,
er inspirert fra koden på denne nettsiden:
http://www.cryptnet.net/fdp/gtk/exec.html
koden på den siden er lisensiert under GPL med copyright V. Alex Brennen [vab@metanet.org] .

7)
Ikonene i images/{16|22|SVG mappene kommer fra Tango ikonsettet
tilgjengelig fra: http://tango.freedesktop.org/Tango_Icon_Library
Disse ikonene er lisensiert under &quot;Creative Commons Attribution Share-Alike license&quot;.
Vi mener at vi imøtekommer kravene i denne lisensen, selv om vi bruker 
ikonene inni umtsmon binærfila.

8)
Flere utbedringer tilknyttet komiplasjon og brukergrensesnittet er fikset av Gerald Pfeifer.

9)
Utbedring i deteksjonen av Vodafone 3G  er utført av dj9pz (Arnd Gronenberg)

10)
Utbedring for Huawei E220 er utført av Norbert Preining (norbusan)

11)
Mye testing og oversettelse til tysk er utført av Lubomir Schmidt

12)
Mer oversettelse til tysk av Philipp Kießler og Dietmar Maurer.

13)
Oversettelse til Portugisk (pt_PT) av Pedro Algarvio.

14)
En del utbedringer av Carsten Clasohm

15)
Flere forbedringer i grensesnittet av Danny Kukawka.




Hvis du har oppdaget at vi har glemt noen, vennligst kontakt forfatteren gjennom Sourceforge  sin nettside:
http://www.sourceforge.net/projects/umtsmon

                                                                                                                                                                             </translation>
    </message>
</context>
<context>
    <name>EnterPIN</name>
    <message>
        <source>Enter PIN</source>
        <translation>Tast inn PIN-koden</translation>
    </message>
    <message>
        <source>Info</source>
        <translation>Info</translation>
    </message>
    <message>
        <source>Please enter the PIN of your 
SIM card</source>
        <translation>Tast inn PIN-koden til ditt SIM-kort</translation>
    </message>
    <message>
        <source>PIN</source>
        <translation>PIN-kode</translation>
    </message>
    <message>
        <source>Enter the PIN of your SIM-Card.</source>
        <translation>Tast inn PIN-koden til ditt SIM-kort.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>This is not a valid PIN code.</source>
        <translation>Dette er ikke en gyldig PIN-kode.</translation>
    </message>
</context>
<context>
    <name>EnterPUK</name>
    <message>
        <source>Please enter your PUK code</source>
        <translation>Tast inn din PUK-kode</translation>
    </message>
    <message>
        <source>PUK of your SIM card</source>
        <translation>Tast inn PUK-koden til ditt SIM-kort</translation>
    </message>
    <message>
        <source>Please enter the PUK-Code of your SIM card</source>
        <translation>Tast inn din PUK-kode</translation>
    </message>
    <message>
        <source>Enter here the PUK of your SIM card</source>
        <translation>Tast inn din PUK-kode</translation>
    </message>
    <message>
        <source>The new PIN</source>
        <translation>Den nye PIN-koden</translation>
    </message>
    <message>
        <source>Enter here the new PIN for your SIM card</source>
        <translation>Tast inn din nye PIN-kode</translation>
    </message>
    <message>
        <source>Enter here the new PIN of your SIM card</source>
        <translation>Tast inn din nye PIN-kode</translation>
    </message>
    <message>
        <source>Info</source>
        <translation>Info</translation>
    </message>
    <message>
        <source>You have entered a wrong PIN  3 times
Please Enter the PUK-Code.</source>
        <translation>Du tastet inn feil PIN-kode  3 ganger
Tast inn PUK-kode.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>PIN or PUK was not valid!</source>
        <translation>PIN-kode eller PUK-kode var ikke gyldig!</translation>
    </message>
    <message>
        <source>PIN needed after PUK?
That&apos;s unexpected!
Please mail info to: umtsmon-develop@sourceforge.net</source>
        <translation>Trenger du virkelig PIN-kode etter PUK-koden?
Det er uvanlig!
Vennligst send e-post med informasjon til: umtsmon-develop@sourceforge.net</translation>
    </message>
    <message>
        <source>success :-)</source>
        <translation>suksess</translation>
    </message>
    <message>
        <source>PUK was not correct.</source>
        <translation>PUK-koden var feil.</translation>
    </message>
    <message>
        <source>Something went wrong in the PIN procedures
umtsmon cannot recover
Please try a restart and/or
insert the SIM card in a GSM phone</source>
        <translation>Noe gikk galt med behandlingen av PIN-koden.
umtsmon klarer ikke å starte
Vennligst restart og/eller
sett inn SIM-kortet i en mobiltelefon</translation>
    </message>
</context>
<context>
    <name>InfoDialogUmtsmon</name>
    <message>
        <source>Info</source>
        <translation type="obsolete">Info</translation>
    </message>
    <message>
        <source>SIM / Device</source>
        <translation>SIM / Kort</translation>
    </message>
    <message>
        <source>IMSI</source>
        <translation>IMSI</translation>
    </message>
    <message>
        <source> - no Value -</source>
        <translation> -  Ikke tilgjengelig -</translation>
    </message>
    <message>
        <source>Manufacturer</source>
        <translation>Produsent</translation>
    </message>
    <message>
        <source>Card-Type</source>
        <translation>Korttype</translation>
    </message>
    <message>
        <source>IMEI</source>
        <translation>IMEI</translation>
    </message>
    <message>
        <source>Revision</source>
        <translation>Revisjon</translation>
    </message>
    <message>
        <source>Linux Kernel</source>
        <translation>Linuxkjerne</translation>
    </message>
    <message>
        <source>Version</source>
        <translation>Versjon</translation>
    </message>
    <message>
        <source>no information available</source>
        <translation>ingen informasjon tilgjengelig</translation>
    </message>
    <message>
        <source>modules</source>
        <translation>moduler</translation>
    </message>
    <message>
        <source>module</source>
        <translation>modul</translation>
    </message>
    <message>
        <source>size</source>
        <translation>størrelse</translation>
    </message>
    <message>
        <source>use count</source>
        <translation>brukt</translation>
    </message>
    <message>
        <source>devices</source>
        <translation>enheter</translation>
    </message>
    <message>
        <source>major nr</source>
        <translation>major nr</translation>
    </message>
    <message>
        <source>device</source>
        <translation>enhet</translation>
    </message>
    <message>
        <source>System</source>
        <translation>System</translation>
    </message>
    <message>
        <source>pppd version:</source>
        <translation>pppd versjon:</translation>
    </message>
    <message>
        <source>distro release:</source>
        <translation>linux distribusjon:</translation>
    </message>
    <message>
        <source>- no Value -</source>
        <translation>-  Ikke tilgjengelig -</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Lukk</translation>
    </message>
    <message>
        <source>Fetching device informations...</source>
        <translation type="obsolete">Henter informasjon fra kort ...</translation>
    </message>
    <message>
        <source>No answer from device.</source>
        <translation>Fikk ikke noe svar.</translation>
    </message>
    <message>
        <source>System Info</source>
        <translation>System Info</translation>
    </message>
    <message>
        <source>AT port</source>
        <translation>AT port</translation>
    </message>
    <message>
        <source>umtsmon driver name</source>
        <translation>umtsmon drivernavn</translation>
    </message>
    <message>
        <source>PPP port</source>
        <translation>PPP port</translation>
    </message>
    <message>
        <source>Fetching device information...</source>
        <translation>Henter informasjon om modem ...</translation>
    </message>
</context>
<context>
    <name>LCDDisplay</name>
    <message>
        <source>n/a</source>
        <translation>N/A</translation>
    </message>
    <message>
        <source>Network:</source>
        <translation type="obsolete">Nettverk:</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Profil:</translation>
    </message>
    <message>
        <source>Up:</source>
        <translation type="obsolete">Opp:</translation>
    </message>
    <message>
        <source>Down:</source>
        <translation type="obsolete">Ned:</translation>
    </message>
    <message>
        <source>Total traffic:</source>
        <translation type="obsolete">Total trafikk:</translation>
    </message>
    <message>
        <source>Operator:</source>
        <translation type="obsolete">Operatør:</translation>
    </message>
    <message>
        <source>%1h %2m</source>
        <translation>%1h %2m</translation>
    </message>
    <message>
        <source>%1m %2s</source>
        <translation>%1m %2s</translation>
    </message>
    <message>
        <source>%1d %2h</source>
        <translation>%1d %2h</translation>
    </message>
</context>
<context>
    <name>ManageProfiles</name>
    <message>
        <source>Manage Profiles</source>
        <translation>Behandle profiler</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>This area shows the profiles that you can use to connect with</source>
        <translation>Her ser de de forskjellige profilene du kan koble deg opp med</translation>
    </message>
    <message>
        <source>&amp;Add profile...</source>
        <translation>&amp;Legg til profil ...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>Create another profile</source>
        <translation>Lag en ny profil</translation>
    </message>
    <message>
        <source>&amp;Edit profile...</source>
        <translation>&amp;Rediger profil ...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>change an existing profile</source>
        <translation>endre en eksisterende profil</translation>
    </message>
    <message>
        <source>&amp;Set as active</source>
        <translation>&amp;Sett som standardprofil</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>make the selected profile active</source>
        <translation>marker at dette er din standardprofil</translation>
    </message>
    <message>
        <source>&amp;Delete profile</source>
        <translation>S&amp;lett profil</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>delete the currently selected profile</source>
        <translation>slett den markerte profilen</translation>
    </message>
    <message>
        <source>please_replace_me</source>
        <translation>bytt_meg_ut</translation>
    </message>
</context>
<context>
    <name>NetworkOperatorWizard</name>
    <message>
        <source>Network Operator Wizard</source>
        <translation>Veiviser for oppsett av nettverk</translation>
    </message>
    <message>
        <source>Page 1: Welcome</source>
        <translation>Side 1: Velkommen</translation>
    </message>
    <message>
        <source>&lt;b&gt;&lt;p align=&quot;center&quot;&gt;Welcome to the&lt;br&gt; 
Network Operator Wizard&lt;/p&gt;&lt;/b&gt;
&lt;p&gt;This wizard will help you choose an operator.&lt;/p&gt;
&lt;p&gt;Please note: when abroad, usually all operators will accept
 your connection and charge you &quot;roaming fees&quot; - 
much more expensive.&lt;/P&gt;</source>
        <translation>&lt;b&gt;&lt;p align=&quot;center&quot;&gt;Velkommen til&lt;br&gt;
Network Operator Wizard&lt;/p&gt;&lt;/b&gt;
&lt;p&gt;Denne guiden til hjelpe deg med å velge en operatør.&lt;/p&gt;
&lt;p&gt;Viktig: Når du er i utlandet så vil de fleste lokale operatører
akseptere deg som kunde, og du vil bli fakturert etter &quot;roaming fees&quot; 
disse prisene er skyhøye!&lt;/P&gt;
</translation>
    </message>
    <message>
        <source>Page 2: Who picks the operator?</source>
        <translation>Side 2: Hvem skal velge operatør?</translation>
    </message>
    <message>
        <source>who picks?</source>
        <translation>hvem skal velge?</translation>
    </message>
    <message>
        <source>have the card decide</source>
        <translation>la kortet velge</translation>
    </message>
    <message>
        <source>let me select an operator</source>
        <translation>la meg velge operatør</translation>
    </message>
    <message>
        <source>&lt;p&gt;Your  SIM card contains
 a &quot;preferred&quot; set of abroad operators.
The device can select these automatically. 
(&quot;Preferred&quot; doesn&apos;t necessarily mean lowest cost)&lt;/p&gt;

&lt;p&gt;Please note: if that &quot;preferred&quot; operator 
has bad coverage, you won&apos;t have 
a connection unless you manually switch.&lt;/p&gt;

&lt;p&gt;However, on manual selection, the device won&apos;t 
switch unless you tell it to - moving to another country 
requires running this wizard again.  &lt;/p&gt;</source>
        <translation>&lt;p&gt;Ditt SIM-kort inneholder
et sett av foretrukne utenlandske operatører.
Sim-kortet ditt kan automatisk velge hvilken operatør den vil bruke.
Viktig: Den operatør som SIM-kortet velger er ikke nødvendigvis den billigste!&lt;/p&gt;

&lt;p&gt;Hvis den operatøren som SIM-kortet automatisk velger har dårlig dekkning, 
så vil du ikke få forbindelse før du manuelt velger en operatør med bedre dekkning.&lt;/p&gt;

&lt;p&gt;Men, når du velger manuelt, så må du huske på å pånytt
velge operatør hver gang du veksler land.  &lt;/p&gt;</translation>
    </message>
    <message>
        <source>Page 3: Get network strengths?</source>
        <translation>Side 3: Hente signalstyrken?</translation>
    </message>
    <message>
        <source>&lt;p&gt;After searching for operators, we will start getting signal strengths.&lt;/p&gt;

&lt;p&gt;This will however significantly increase the search time!
&lt;/p&gt;</source>
        <translation>&lt;p&gt;Etter at vi har begynt å lete etter nettverksoperatører så vil vi se signalstyrken.&lt;/p&gt;

&lt;p&gt;Men merk at dette vil gjøre at oppsettet kan ta mye lengre tid
&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Don&apos;t get signal strengths</source>
        <translation>Ikke hent signalstyrke</translation>
    </message>
    <message>
        <source>&lt;p align=&quot;center&quot;&gt;retrieving signal strengths not implemented yet&lt;/p&gt;</source>
        <translation>&lt;p align=&quot;center&quot;&gt;kalkulering av signalstyrke er ennå ikke implementert&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Page 4: Searching for networks...</source>
        <translation>Side 4: Leter etter nettverk ...</translation>
    </message>
    <message>
        <source>&lt;p align=&quot;center&quot;&gt;Searching for networks...&lt;/p&gt;

&lt;p align=&quot;center&quot;&gt;Please wait...&lt;/p&gt;</source>
        <translation>&lt;p align=&quot;center&quot;&gt;Leter etter nettverk ...&lt;/p&gt;

&lt;p align=&quot;center&quot;&gt;Vennligst vent ...&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Page 5: Select a Network Operator</source>
        <translation>Side 5: Velg en nettverksoperatør</translation>
    </message>
    <message>
        <source>Please select your network:</source>
        <translation>Velg ditt nettverk:</translation>
    </message>
    <message>
        <source>Still getting network strengths...</source>
        <translation>Henter fremdeles signalstyrke ...</translation>
    </message>
    <message>
        <source>Operator</source>
        <translation>Operatør</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Signal</source>
        <translation>Signal</translation>
    </message>
    <message>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <source>This table lists the operators to choose from.</source>
        <translation>Dette er listen med operatører du kan velge blant.</translation>
    </message>
    <message>
        <source>Page 6: Connecting to operator...</source>
        <translation>Side 6: Kobler opp til operatør ...</translation>
    </message>
    <message>
        <source>&lt;p align=&quot;center&quot;&gt;Connecting to your network...&lt;/p&gt;

&lt;p align=&quot;center&quot;&gt;Please wait...&lt;/p&gt;</source>
        <translation>&lt;p align=&quot;center&quot;&gt;Kobler opp ...&lt;/p&gt;

&lt;p align=&quot;center&quot;&gt;Vennligst vent ...&lt;/p&gt;</translation>
    </message>
    <message>
        <source>The End</source>
        <translation>Slutt</translation>
    </message>
    <message>
        <source>&lt;b&gt;&lt;p align=&quot;center&quot;&gt;Network Operator
Wizard&lt;br&gt;completed&lt;/p&gt;&lt;/b&gt;
&lt;br&gt;&lt;br&gt;
&lt;p&gt;Have fun on the internet!&lt;/P&gt;</source>
        <translation>&lt;b&gt;&lt;p align=&quot;center&quot;&gt;Nettverksoperatør
Wizard&lt;br&gt;ferdig&lt;/p&gt;&lt;/b&gt;
&lt;br&gt;&lt;br&gt;
&lt;p&gt;Ha de trivelig på internett.&lt;/P&gt;</translation>
    </message>
    <message>
        <source>No network operators were found.
You might want to try again.</source>
        <translation>Fant ingen nettverksoperatører.
Du må prøve igjen.</translation>
    </message>
</context>
<context>
    <name>NewProfileDialog</name>
    <message>
        <source>New Profile</source>
        <translation>Ny profil</translation>
    </message>
    <message>
        <source>Profile name</source>
        <translation>Profilnavn</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>please enter a new profile name</source>
        <translation>Skriv inn det nye navnet på profilen</translation>
    </message>
    <message>
        <source>This profile name is invalid, please change.</source>
        <translation>Dette er ikke et gyldig profilnavn, bruk et annet.</translation>
    </message>
</context>
<context>
    <name>PPPErrorLogsDialog</name>
    <message>
        <source>PPP Error Logs</source>
        <translation>PPP feillogger</translation>
    </message>
    <message>
        <source>Problem</source>
        <translation>Problem</translation>
    </message>
    <message>
        <source>&lt;b&gt;Connection failed&lt;/b&gt;&lt;br&gt;&lt;br&gt;

Unfortunately, PPP could not make a connection.&lt;br&gt;&lt;br&gt;
On the next two tabs,  you&apos;ll find the information PPP returned. Please check if you have the right PPP options enabled. If you need more information, please re-run this program from the command line with the -v4 argument.</source>
        <translation>&lt;b&gt;Oppkobling feilet&lt;/b&gt;&lt;br&gt;&lt;br&gt;

Dessverre, PPP kunne ikke sette opp en forbindelse.&lt;br&gt;&lt;br&gt;
I de to neste arkfanene,  så finner du feilmeldingen fra PPP. Se på disse feilmeldingene og sjekk om du bruker de korrekte PPP valgene. Hvis du trenger mer informasjon, så start programet fra kommandolinja med -v4 argumentet.</translation>
    </message>
    <message>
        <source>PPP stdout</source>
        <translation>PPP stdout</translation>
    </message>
    <message>
        <source>Copy this information into e.g. an e-mail.</source>
        <translation>Kopier denne informasjonen f.eks inn i en e-post.</translation>
    </message>
    <message>
        <source>PPP stderr</source>
        <translation>PPP stderr</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
</context>
<context>
    <name>PopupWithMemory</name>
    <message>
        <source>MyDialog</source>
        <translation>Min Dialog</translation>
    </message>
    <message>
        <source>textLabel1</source>
        <translation></translation>
    </message>
    <message>
        <source>Do not show this message again</source>
        <translation>Ikke vis denne beskjeden igjen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>&lt;p align=&quot;center&quot;&gt;&lt;b&gt;APN info&lt;/b&gt;&lt;/p&gt; &lt;p align=&quot;left&quot;&gt;For internet-connection, most providers accept an empty APN-Name.&lt;br&gt;So you&apos;ll &lt;b&gt;not&lt;/b&gt; have to fill in APN, username,password.&lt;/p&gt; &lt;p align=&quot;center&quot;&gt;If your provider requires an APN, you can find a list of those here:&lt;br&gt; &lt;b&gt;http://www.pinstack.com/carrier_settings_apn_gateway.html&lt;/b&gt;&lt;/p&gt; &lt;p align=&quot;left&quot;&gt;For Private APNs, please ask your provider for support!&lt;/p&gt;</source>
        <translation>&lt;p align=&quot;center&quot;&gt;&lt;b&gt;APN info&lt;/b&gt;&lt;/p&gt; &lt;p align=&quot;left&quot;&gt;For internet-oppkobling, så vil de fleste operatører akseptere et tomt APN-navn.&lt;br&gt;Du trenger &lt;b&gt;ikke&lt;/b&gt; å fylle inn APN, brukernavn, passord.&lt;/p&gt; &lt;p align=&quot;center&quot;&gt;Hvis din operatør forlanger et APN, så finner du her en liste med slike:&lt;br&gt; &lt;b&gt;http://www.pinstack.com/carrier_settings_apn_gateway.html&lt;/b&gt;&lt;/p&gt; &lt;p align=&quot;left&quot;&gt;For private APN, så hør med din operatør.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>n/a</source>
        <translation>N/A</translation>
    </message>
    <message>
        <source>Could not write to device (anymore)</source>
        <translation>Kunne ikke (lenger) skrive til kortet</translation>
    </message>
    <message>
        <source>Your device was recognised as a usbserial device,
yet the corresponding kernel module is not running on your system.

Do you want to load that module now?</source>
        <translation>Ditt kort ble gjenkjent som en usbserial enhet,
men den nødvendige linuxkjerne modulen er ikke lastet på ditt system.

Vil du laste denne modulen nå?</translation>
    </message>
    <message>
        <source>SIM card is moaning about something I don&apos;t know about:
</source>
        <translation type="obsolete">SIM-kortet klager over noe jeg ikke skjønner:
</translation>
    </message>
    <message>
        <source>
Please insert the SIM into a regular phone and check it...</source>
        <translation type="obsolete">
Forsøk å sette SIM-korten inn i en vanlig mobiltelefon og sjekk at det virker ...</translation>
    </message>
    <message>
        <source>Unknown error - please review logs</source>
        <translation>Ukjent feil -  vennligst ta en titt i feilloggene</translation>
    </message>
    <message>
        <source>Waiting for SIM-Interface...</source>
        <translation>Venter på SIM-kortet ...</translation>
    </message>
    <message>
        <source>SIM card handling resulted in this error:
      </source>
        <translation type="obsolete">Behandling av SIM-kortet resulterte i denne feilen:
      </translation>
    </message>
    <message>
        <source>Trying to set PIN/PUK</source>
        <translation>Forsøker å sette PIN/PUK</translation>
    </message>
    <message>
        <source>Apparently there is already a pppd process running.
This prevents umtsmon from detecting your card,
umtsmon will now close.</source>
        <translation>Det ser ut til at det allerede er en pppd prosess som kjører.
Dette gjør at umtsmon ikke kan detektere ditt kort,
umtsmon vil avslutte.</translation>
    </message>
    <message>
        <source>No device could be found, the application will close.
If you need more info, please re-run from the command line.</source>
        <translation type="obsolete">Fant ikke noe kort, programet avslutter.
Hvis du trenger mer info, så start programet fra kommandolinja.</translation>
    </message>
    <message>
        <source>Searching...</source>
        <translation>Søker ...</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>We think we found your device, but it shows too little ports.
This usually indicated that your device needs &apos;switching&apos;.
However, no switch application (usb_modeswitch) was found.
we will continue, but this might cause trouble later on...</source>
        <translation>umtsmon tror at ditt modem er funnet, men det er ikke nok porter på det.
Dette betyr vanligvis at du må &quot;switche&quot; modemet.
For å kunne &quot;switche&quot; modemet så trengs programet usb_modeswitch, men det er ikke installert.
Vi kan fortsette, men det er ikke optimalt ...</translation>
    </message>
    <message>
        <source>umtsmon detected that you need to switch your device.
Click Yes if you agree.
 Note: this does not work properly for all devices yet!</source>
        <translation>umtsmon oppdaget at du må &quot;switche&quot;  modemet ditt.
Klikk på Ja hvis det er greit.
Merk: dette fungerer ikke på alle modem!
</translation>
    </message>
    <message>
        <source>Welcome to umtsmon.</source>
        <translation>Velkommen til umtsmon.</translation>
    </message>
    <message>
        <source>umtsmon detected that you need to &apos;modeswitch&apos; your device.
However, you do not have any switch program installed.
We suggest you install usb_modeswitch and edit its config file</source>
        <translation>umtsmon har oppdaget at du må  &apos;modeswitche&apos; ditt modem.
Men, du har ikke noe program installert som kann gjøre det for deg.
Vi anbefaler at du installerer usb_modeswitch, og redigerer dens konfigurasjonsfil</translation>
    </message>
    <message>
        <source>SIM card is moaning about something I don&apos;t know about:
&quot;%1&quot;
Please insert the SIM into a regular phone and check it...</source>
        <translation>SIM-kortet klager over jeg jeg ikke forstår:
&quot;%1&quot;
Sett in ditt SIM-kort  i en vanlig mobiltelefon og sjekk at det  er i orden ...</translation>
    </message>
    <message>
        <source>SIM card handling resulted in this error:
  &quot;%1&quot;, sorry.
umtsmon will now close.</source>
        <translation>Håndteringe av SIM-kortet  resulterte i denne feilen:
  &quot;%1&quot;,  beklager.
umtsmon vil  avslutte.</translation>
    </message>
    <message>
        <source>!!! missing binary</source>
        <translation>!!! mangler program</translation>
    </message>
    <message>
        <source>%1 couldn&apos;t find binary %2, which is required for full operation.</source>
        <translation>%1 fant ikke programmet %2, som er nødvendig for at alt skal virke.</translation>
    </message>
    <message>
        <source>user does not have write access</source>
        <translation>bruker har ikke skrivetilgang</translation>
    </message>
    <message>
        <source>user does not have read access</source>
        <translation>bruker har ikke lesetilgang</translation>
    </message>
    <message>
        <source>read/write access is OK</source>
        <translation>lese- og skriverettigheter OK</translation>
    </message>
    <message>
        <source>user does not have read nor write access (or something else has a lock)</source>
        <translation>bruker har hverken lese eller skrive rettigheter (eller noe annet har låst dem)</translation>
    </message>
    <message>
        <source>not found on file system</source>
        <translation>ble ikke funnet på filsystemet</translation>
    </message>
    <message>
        <source>Modem Communication Ports</source>
        <translation>Modem Communication Ports</translation>
    </message>
    <message>
        <source>You run as root - no need for groups</source>
        <translation>Du kjører som root, trenger ikke grupper</translation>
    </message>
    <message>
        <source>Member of the following user groups</source>
        <translation>Medlem av følgende grupper</translation>
    </message>
    <message>
        <source>usually one needed to access the device</source>
        <translation>vanligvis så  trengs tilgang til et modem</translation>
    </message>
    <message>
        <source>is member</source>
        <translation>er medlem</translation>
    </message>
    <message>
        <source>not a member - not fixable by umtsmon</source>
        <translation>ikke medlem  - umtsmon kan ikke ordne det</translation>
    </message>
    <message>
        <source>required if you want to make connections</source>
        <translation>nødvendig om du vil koble deg opp</translation>
    </message>
    <message>
        <source>only required for PCMCIA cards</source>
        <translation>trengs kun for PCMCIAkort</translation>
    </message>
    <message>
        <source>if this app is present, you probably need it.</source>
        <translation>hvis  dette programet er installert, så trenger du det sikkert.</translation>
    </message>
    <message>
        <source>Your APN contains illegal characters.
Please go back to your Profile page and change the APN.</source>
        <translation>Din APN inneholder ugyldige tegn.
Gå tilbake til din profile og endre det du skrev inn for APN.</translation>
    </message>
    <message>
        <source>Warning: You did not specify an &apos;APN&apos; in your profile.
This may work ok, it may not - depending on your network operator.</source>
        <translation>Advarsel: Du satt ikke opp noe APN i din  profil.
Det kan allikevel virke, eller kanskje ikke  -  alt avhengig av din operatør.</translation>
    </message>
    <message>
        <source>No Connection Class defined yet</source>
        <translation>Ingen Connection Class definert ennå</translation>
    </message>
    <message>
        <source>HSO device - no pppd required</source>
        <translation>HSO modem, trenger ikke pppd</translation>
    </message>
    <message>
        <source> message</source>
        <translation> beskjed</translation>
    </message>
</context>
<context>
    <name>RadioPreferences</name>
    <message>
        <source>Radio Preferences</source>
        <translation>Radio instillinger</translation>
    </message>
    <message>
        <source>Limit radio types:</source>
        <translation>Begrens radiotyper til:</translation>
    </message>
    <message>
        <source>if your card supports this feature, you can limit the card to a certain radio technology. This is useful if you have a fringe coverage, e.g. the card is jumping between UMTS and GPRS because neither is really strong.</source>
        <translation>Hvis ditt kort støtter denne egenskapen,  så kan du begrense ditt kort til å bruke en bestemt radio teknologi. Dette er nyttig hvis 
du befinner deg i et grenseområde, dvs kortet ditt hopper mellom UMTS og GPRS fordi ingen av dem er sterke nok.</translation>
    </message>
    <message>
        <source>(changing radio types whilst a PPP connection exists will fail)</source>
        <translation>(endring av radiotype mens en PPP forbindelse er aktiv vil feile)</translation>
    </message>
    <message>
        <source>automatic update of signal strength and download statistics</source>
        <translation type="obsolete">automatisk oppdatering av signalstyrke og nedlastnings statistikk</translation>
    </message>
    <message>
        <source>disable this if you don&apos;t want umtsmon to check the signal strength and the byte counts every three seconds</source>
        <translation type="obsolete">Skru av denne hvis du ikke vil at umtsmon skal sjekke signalstyrken og telle antall bytes hvert tredje sekund</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>Failed to set your radio technology preference.</source>
        <translation>Klarte ikke å bruke din foretrukne radio teknologi.</translation>
    </message>
</context>
<context>
    <name>SetLanguageDialog</name>
    <message>
        <source>Set Language</source>
        <translation>Velg språk</translation>
    </message>
    <message>
        <source>Please select your language. 
This setting will be active after a restart.</source>
        <translation>Velg  ønsket språk.
Dette vil ta effekt ved neste start.</translation>
    </message>
    <message>
        <source>Available languages</source>
        <translation>Tilgjengelige språk</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>Selecting the language failed ?!?</source>
        <translation>Klarte ikke å bruke valgt språk ?!?</translation>
    </message>
</context>
<context>
    <name>UIState</name>
    <message>
        <source>Detecting device %1</source>
        <translation>
Oppdager modem %1</translation>
    </message>
    <message>
        <source>SIM check %1</source>
        <translation>SIM-kort sjekk  %1</translation>
    </message>
    <message>
        <source>No device</source>
        <translation>Ikke noe modem</translation>
    </message>
    <message>
        <source>Registering %1</source>
        <translation>Registrerer %1</translation>
    </message>
    <message>
        <source>Connecting %1</source>
        <translation>Kobler  til  %1</translation>
    </message>
    <message>
        <source>Disconnecting %1</source>
        <translation>Kobler fra %1</translation>
    </message>
    <message>
        <source>Before you connect, please create a &quot;profile&quot;.</source>
        <translation>Før du kan koble deg opp, så må du lage en &quot;profil&quot;.</translation>
    </message>
    <message>
        <source>No device (yet)... (%1)</source>
        <translation>Modem ikke funnet (ennå) ...(%1)</translation>
    </message>
</context>
<context>
    <name>YesNoWithMemory</name>
    <message>
        <source>MyDialog</source>
        <translation>Min Dialog</translation>
    </message>
    <message>
        <source>textLabel1</source>
        <translation></translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Alt+Y</source>
        <translation>Alt+Y</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nei</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Do not show this message again</source>
        <translation>Ikke vis denne beskjede igjen</translation>
    </message>
</context>
<context>
    <name>helperlistgui</name>
    <message>
        <source>Setup external applications</source>
        <translation>Oppsett av eksterne applikasjoner</translation>
    </message>
    <message>
        <source>Several helper applications and file permission settings are needed for proper function.
 Below is a list of those which also specifies whether everything is ok.  If is not, &quot;Fix&quot; will try to change the appropriate setting.</source>
        <translation>Ytterligere hjelpeprogram og endringer i filrettigheter trengs.
Her er en liste med detaljert status.  Hvis noe ikke er iorden, så vil &quot;Korriger&quot; forsøke å rette opp i det.</translation>
    </message>
    <message>
        <source>#</source>
        <translation>#</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Sti</translation>
    </message>
    <message>
        <source>Version</source>
        <translation>Versjon</translation>
    </message>
    <message>
        <source>suid?</source>
        <translation>suid?</translation>
    </message>
    <message>
        <source>Remarks</source>
        <translation>Bemerkninger</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hjelp</translation>
    </message>
    <message>
        <source>F1</source>
        <translation>F1</translation>
    </message>
    <message>
        <source>&amp;Fix</source>
        <translation>&amp;Korriger</translation>
    </message>
    <message>
        <source>Alt+F</source>
        <translation>Alt+F</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorer</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>don&apos;t adjust anything and continue running</source>
        <translation>ikke foreta noen endringer, fortsett å kjøre</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Avslutt</translation>
    </message>
    <message>
        <source>Alt+Q</source>
        <translation>Alt+Q</translation>
    </message>
    <message>
        <source>stop running the application immediately</source>
        <translation>avslutt applikasjonen umiddelbart</translation>
    </message>
</context>
<context>
    <name>mainwindow</name>
    <message>
        <source>umtsmon mainwindow</source>
        <translation>umtsmon hovedvindu</translation>
    </message>
    <message>
        <source>&amp;Connection</source>
        <translation>&amp;Oppkobling</translation>
    </message>
    <message>
        <source>&amp;SMS</source>
        <translation>&amp;SMS</translation>
    </message>
    <message>
        <source>&amp;PIN-Settings</source>
        <translation type="obsolete">&amp;PIN-kode</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hjelp</translation>
    </message>
    <message>
        <source>Toolbar</source>
        <translation>Verktøylinje</translation>
    </message>
    <message>
        <source>manage profiles</source>
        <translation>Behandle profiler</translation>
    </message>
    <message>
        <source>Connect with default Profile</source>
        <translation>Koble til med standardprofil</translation>
    </message>
    <message>
        <source>disconnect</source>
        <translation>Koble fra</translation>
    </message>
    <message>
        <source>create a new sms</source>
        <translation>Skriv en ny sms</translation>
    </message>
    <message>
        <source>refresh operator/signal/radio/statistics on the display</source>
        <translation type="obsolete">Oppfrisk display</translation>
    </message>
    <message>
        <source>Connect</source>
        <translation>Koble til </translation>
    </message>
    <message>
        <source>&amp;Connect</source>
        <translation>&amp;Koble til </translation>
    </message>
    <message>
        <source>Ctrl+C</source>
        <translation>Ctrl+C</translation>
    </message>
    <message>
        <source>Disconnect</source>
        <translation>Koble fra</translation>
    </message>
    <message>
        <source>&amp;Disconnect</source>
        <translation>K&amp;oble fra</translation>
    </message>
    <message>
        <source>Ctrl+D</source>
        <translation>Ctrl+D</translation>
    </message>
    <message>
        <source>Power down card</source>
        <translation>Skru av kortet</translation>
    </message>
    <message>
        <source>P&amp;ower down card</source>
        <translation>&amp;Skru av kortet</translation>
    </message>
    <message>
        <source>Reset card</source>
        <translation>Reset kortet</translation>
    </message>
    <message>
        <source>R&amp;eset card</source>
        <translation>R&amp;eset kortet</translation>
    </message>
    <message>
        <source>New SMS</source>
        <translation>Ny SMS</translation>
    </message>
    <message>
        <source>&amp;New SMS...</source>
        <translation>&amp;Ny SMS ...</translation>
    </message>
    <message>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <source>Enable PIN</source>
        <translation>Skru på bruken av PIN-kode</translation>
    </message>
    <message>
        <source>&amp;Enable PIN</source>
        <translation type="obsolete">S&amp;kru på bruken av PIN-kode</translation>
    </message>
    <message>
        <source>&amp;Disable PIN</source>
        <translation type="obsolete">&amp;Skru av bruken av PIN-kode</translation>
    </message>
    <message>
        <source>Disable PIN</source>
        <translation>Skru av bruken av PIN-kode</translation>
    </message>
    <message>
        <source>Change PIN</source>
        <translation>Bytt PIN-kode</translation>
    </message>
    <message>
        <source>&amp;Change PIN</source>
        <translation type="obsolete">&amp;Bytt PIN-kode</translation>
    </message>
    <message>
        <source>System Info</source>
        <translation>System Info</translation>
    </message>
    <message>
        <source>System &amp;Info</source>
        <translation>System &amp;Info</translation>
    </message>
    <message>
        <source>F2</source>
        <translation>F2</translation>
    </message>
    <message>
        <source>Autodetection log</source>
        <translation>Autodeteksjon log</translation>
    </message>
    <message>
        <source>Autodetection &amp;log</source>
        <translation>Autodeteksjon &amp;log</translation>
    </message>
    <message>
        <source>About</source>
        <translation>Om</translation>
    </message>
    <message>
        <source>&amp;About APPNAME</source>
        <translation>&amp;Om APPNAME</translation>
    </message>
    <message>
        <source>F3</source>
        <translation>F3</translation>
    </message>
    <message>
        <source>Help</source>
        <translation>Hjelp</translation>
    </message>
    <message>
        <source>F1</source>
        <translation>F1</translation>
    </message>
    <message>
        <source>Manage Profiles</source>
        <translation>Behandle profiler</translation>
    </message>
    <message>
        <source>Manage &amp;Profiles...</source>
        <translation>Behandle &amp;profiler ...</translation>
    </message>
    <message>
        <source>Ctrl+P</source>
        <translation>Ctrl+P</translation>
    </message>
    <message>
        <source>Quit APPNAME</source>
        <translation>Avslutt APPNAME</translation>
    </message>
    <message>
        <source>&amp;Quit APPNAME</source>
        <translation>&amp;Avslutt umtsmon</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>new item</source>
        <translation type="obsolete">ny</translation>
    </message>
    <message>
        <source>Select network operator</source>
        <translation>Velg nettverksoperatør</translation>
    </message>
    <message>
        <source>&amp;Select network operator...</source>
        <translation>&amp;Velg nettverksoperatør ...</translation>
    </message>
    <message>
        <source>Radio Preferences</source>
        <translation>Radio instillinger</translation>
    </message>
    <message>
        <source>&amp;Radio Preferences...</source>
        <translation>&amp;Radio instillinger ...</translation>
    </message>
    <message>
        <source>Disabling PIN protection failed.
Try again, and be sure to use the correct current PIN!</source>
        <translation>Klarte ikke å skru av bruk av PIN-kode beskyttelse.
Prøv igjen, vær sikker på at bruker riktig PIN-kode!</translation>
    </message>
    <message>
        <source>Enabling PIN protection failed.
Try again, and be sure to use the correct current PIN!</source>
        <translation>Bruk av PIN-kode beskyttelse feilet.
Prøv igjen, vær sikker på at bruker riktig PIN-kode!</translation>
    </message>
    <message>
        <source>PIN protection was successfull.</source>
        <translation type="obsolete">Ditt SIM-kort er nå beskyttet med PIN-kode.</translation>
    </message>
    <message>
        <source>Please enter your current PIN.</source>
        <translation>Tast inn din nåværende PIN-kode.</translation>
    </message>
    <message>
        <source>Please enter your NEW pin.</source>
        <translation>Tast inn din nye PIN-kode.</translation>
    </message>
    <message>
        <source>Please verify your NEW pin!</source>
        <translation type="obsolete">Tast inn din nye PIN-kode pånytt!</translation>
    </message>
    <message>
        <source>Changing PIN failed.
New PIN and verify PIN were not same, please try again!</source>
        <translation>Klarte ikke å bytte PIN-kode.
Du tastet ikke inn din nye PIN-kode likt begge gangene, prøv igjen!</translation>
    </message>
    <message>
        <source>Changing PIN failed.
Try again, and be sure to use the correct current PIN!</source>
        <translation>Klarte ikke å bytte PIN-kode.
Pøv igjen, husk å bruke korrekt nåværende PIN-kode!</translation>
    </message>
    <message>
        <source>PIN changed successfully.</source>
        <translation>PIN-kode byttet. </translation>
    </message>
    <message>
        <source>You are not connected to a network yet.
SMS functions are unavailable.</source>
        <translation>Du er ikke koblet til et nettverk ennå.
SMS funksjonaliteten er ikke tilgjengelig.</translation>
    </message>
    <message>
        <source>&lt;&lt;PPP did not provide any stdout information&gt;&gt;</source>
        <translation>&lt;&lt;PPP kom ikke med noe informasjon til stdout&gt;&gt;</translation>
    </message>
    <message>
        <source>&lt;&lt;PPP did not provide any stderr information&gt;&gt;</source>
        <translation>&lt;&lt;PPP kom ikke med noe informasjon til stderr&gt;&gt;</translation>
    </message>
    <message>
        <source>You are not connected to a network yet.</source>
        <translation type="obsolete">Du er ikke koblet til noe nettverk ennå.</translation>
    </message>
    <message>
        <source>Setting up PPP Connection</source>
        <translation type="obsolete">Setter opp PPP forbindelse</translation>
    </message>
    <message>
        <source>Failed to start PPP.
Please re-run umtsmon from the commandline with -v5 and check the output.</source>
        <translation>Klarte ikke å starte PPP.
Vennligst start umtsmon fra kommandolinja med argumentet -v5 og sjekk feilmeldingen.</translation>
    </message>
    <message>
        <source>PPP is active, cannot run this in parallel.</source>
        <translation>PPP er allerede aktiv, kan ikke kjøre denne i parallell.</translation>
    </message>
    <message>
        <source>pops up a dialog with system information.</source>
        <translation>sprettoppvindu med systeminformasjon.</translation>
    </message>
    <message>
        <source>Se&amp;ttings</source>
        <translation>Inns&amp;tillinger</translation>
    </message>
    <message>
        <source>PIN Settings</source>
        <translation>PIN-kode innstillinger</translation>
    </message>
    <message>
        <source>  &amp;Enable PIN...</source>
        <translation>  S&amp;kru på PIN-kode ...</translation>
    </message>
    <message>
        <source>  &amp;Disable PIN...</source>
        <translation>  &amp;Skru av PIN-kode ...</translation>
    </message>
    <message>
        <source>  &amp;Change PIN...</source>
        <translation>  &amp;Bytt PIN-kode ...</translation>
    </message>
    <message>
        <source>Set the language</source>
        <translation>Velg språk</translation>
    </message>
    <message>
        <source>Set Language...</source>
        <translation>Velg språk ...</translation>
    </message>
    <message>
        <source>Automatically connect when network is found</source>
        <translation>Koble opp automatisk når et nnetverk blir funnet</translation>
    </message>
    <message>
        <source>Autoconnect</source>
        <translation>Autotilkobling</translation>
    </message>
    <message>
        <source>Sorry, the Info dialog cannot be shown
whilst a PPP connection exists.</source>
        <translation>Beklager,  Info Dialogen kan ikke vises
mens en PPP forbindelse er aktiv.</translation>
    </message>
    <message>
        <source>Disabling PIN protection was successful.</source>
        <translation>Beskyttelse med PIN-kode er avskrudd.</translation>
    </message>
    <message>
        <source>Enabling PIN protection was successful.</source>
        <translation>Beskyttelse med PIN-kode er skrudd på.</translation>
    </message>
    <message>
        <source>Please enter your NEW pin again.</source>
        <translation>Tast inn din nye PIN-kode pånytt.</translation>
    </message>
    <message>
        <source>Your computer just ran into a new month.
Due to limitations of pppd, we cannot account newtraffic to the new month. Please close the connectionand reconnect at your earliest convenience.</source>
        <translation>Klokka på din datamaskin gikk over i en ny måned.
På grunn av begrensninger i pppd, så kan vi ikke føre opp trafikken for denne måneden  separat fra den forrige,  ikke før du koler fra og kobbler opp igjen pånytt.</translation>
    </message>
    <message>
        <source>You are over your data limit treshold.</source>
        <translation>Du har overskredet din grense for datatrafikk.</translation>
    </message>
    <message>
        <source>You are over your time limit treshold.</source>
        <translation>Du har overskredet din grense for tid.</translation>
    </message>
    <message>
        <source>Setting up Internet Connection</source>
        <translation>Setter opp  Internet forbindelse</translation>
    </message>
    <message>
        <source>Please do not change
the profile whilst connected.</source>
        <translation>Ikke endr din
profil mens du er koblet opp.</translation>
    </message>
    <message>
        <source>Sorry, the Info dialog cannot be shown right now.</source>
        <translation>Beklager,  Info Dialogen kan ikke vises  nå.</translation>
    </message>
    <message>
        <source>You can only enable autoconnect once you have made a successful connection.</source>
        <translation>Du kan ikke skru på autooppkoblig før du har hatt en sukessfull  oppbobling.</translation>
    </message>
</context>
<context>
    <name>networkoperator</name>
    <message>
        <source>Network Operator</source>
        <translation type="obsolete">Nettverksoperatør</translation>
    </message>
    <message>
        <source>Find networks</source>
        <translation type="obsolete">Finn nettverk</translation>
    </message>
    <message>
        <source>when disconnected, clicking this button will search for available networks</source>
        <translation type="obsolete">når du er frakoblet, så vil denne knappen starte søking etter tilgjengelige nettverk</translation>
    </message>
    <message>
        <source>Select</source>
        <translation type="obsolete">Velg</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Searching for mobile networks...</source>
        <translation type="obsolete">Leter etter mobilnettverk ...</translation>
    </message>
    <message>
        <source>No network operators were found.
You might want to try again.</source>
        <translation type="obsolete">Fant ingen nettverksoperatører.
Du må prøve igjen.</translation>
    </message>
    <message>
        <source>Select your operator from the above list.</source>
        <translation type="obsolete">Velg din operatør fra listen over.</translation>
    </message>
    <message>
        <source>Connecting to mobile network...</source>
        <translation type="obsolete">Kobler opp til mobilnettverk ...</translation>
    </message>
</context>
<context>
    <name>profiledialog</name>
    <message>
        <source>Edit Profile</source>
        <translation>Rediger profil</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Profilnavn:</translation>
    </message>
    <message>
        <source>advanced PPP Options</source>
        <translation>Ekstra PPP instillinger</translation>
    </message>
    <message>
        <source>noauth</source>
        <translation>noauth</translation>
    </message>
    <message>
        <source>Do not require the remote server to authenticate itself. This option is privileged. Usually disabled.</source>
        <translation>Ikke forlang at den eksterne server skal autentisere seg selv. Dette er en priviligert opsjon. 
Som ofest ikke aktivert.</translation>
    </message>
    <message>
        <source>no ip default</source>
        <translation>no ip default</translation>
    </message>
    <message>
        <source>If enabled, the remote server must supply you an IP Address. Usually disabled.</source>
        <translation>Hvis denne er aktivert, så må den eksterne serveren gi deg en IP-adresse. Som oftest ikke aktivert.</translation>
    </message>
    <message>
        <source>replace default route</source>
        <translation>replace default route</translation>
    </message>
    <message>
        <source>Use the connection for all network traffic when connected. Usually enabled.</source>
        <translation>Bruk denne oppkoblingen for all nettverkstrafikk. Som oftest aktivert.</translation>
    </message>
    <message>
        <source>set default route</source>
        <translation>set default route</translation>
    </message>
    <message>
        <source>Add a default route to the remote server if connected. Usually enabled.</source>
        <translation>Sett en default route for den eksterne serveren hvis du er oppkoblet. Som oftest aktivert.</translation>
    </message>
    <message>
        <source>no header compression</source>
        <translation>no header compression</translation>
    </message>
    <message>
        <source>Header Compression saves a few byte per data packet by skipping information that is already known by the peer. Usually enabled because most cards don&apos;t understand it.</source>
        <translation type="obsolete">Header Compression sparer noen få byte for hver data pakke ved å hoppe over informasjon som allerede er kjent. 
Dette er vanligvis aktivert fordi de fleste kort ikke skjønner dette.</translation>
    </message>
    <message>
        <source>no CCP</source>
        <translation>ingen CCP</translation>
    </message>
    <message>
        <source>Do not negotiate the Compression Control Protocol. Usually disabled - only needed for buggy operators.</source>
        <translation>Ikke forhandle om Compression Control Protocol. 
Vanligvis ikke aktivert - trengs kun for kranglete operatører.</translation>
    </message>
    <message>
        <source>use peer DNS</source>
        <translation>bruk peer DNS</translation>
    </message>
    <message>
        <source>Receive DNS-settings from your Provider. Usually enabled.</source>
        <translation>Motta DNS-oppsett fra din tilbyder. Som oftest aktivert.</translation>
    </message>
    <message>
        <source>login</source>
        <translation>login</translation>
    </message>
    <message>
        <source>APN</source>
        <translation>APN</translation>
    </message>
    <message>
        <source>Username</source>
        <translation>Brukernavn</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Passord</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>monthly data limits</source>
        <translation type="obsolete">Månedlig kvote for datatrafikk</translation>
    </message>
    <message>
        <source>warn when threshold is reached:</source>
        <translation>Advar meg når jeg har nådd:</translation>
    </message>
    <message>
        <source>min 50%, max 100%</source>
        <translation>min 50%, maks 100%</translation>
    </message>
    <message>
        <source>%</source>
        <translation>%</translation>
    </message>
    <message>
        <source>home network:</source>
        <translation>Fast mobiloperatør:</translation>
    </message>
    <message>
        <source>d9999; </source>
        <translation type="obsolete">d9999; </translation>
    </message>
    <message>
        <source>Please enter the megabytes per month that is part of your subscription. This field is for when you are outside your home network - also called roaming.</source>
        <translation>Oppgi antall megabytes per måned som er inkludert i ditt abonnement. 
Dette feltet er for de tilfellene du er utenfor nettet til din faste operatør - også kalt roaming.</translation>
    </message>
    <message>
        <source>roaming:</source>
        <translation>Roaming:</translation>
    </message>
    <message>
        <source>dddd9; </source>
        <comment>numbers only</comment>
        <translation type="obsolete">dddd9; </translation>
    </message>
    <message>
        <source>Please enter the megabytes per month that is part of your subscription. This field is for your home network.</source>
        <translation>Oppgi antall megabytes per måned som du (evt) har i ditt abonnement. 
Dette feltet er for din faste mobiloperatør.</translation>
    </message>
    <message>
        <source>MiB/month</source>
        <translation>MiB/måned</translation>
    </message>
    <message>
        <source>There are unsaved changes to your profile!</source>
        <translation>Det finnes ulagrede endringer i din profil!</translation>
    </message>
    <message>
        <source>Header Compression saves a few byte per data packet by skipping information that is already known by the peer. ALWAYS ENABLED.</source>
        <translation>Header kompresjon sparer noen få byte per pakke ved å utelate informasjon som allerede er kjent av server. ALLTID SKRUDD PÅ.</translation>
    </message>
    <message>
        <source>monthly data or time limits</source>
        <translation>månedlig tidskvote eller datamengdekvote</translation>
    </message>
    <message>
        <source>min/month</source>
        <translation>minutter/måned</translation>
    </message>
    <message>
        <source>if your operator requires a username, fill it in here.&lt;br&gt;If not, you probably want to put something dummy in here.</source>
        <translation>hvis din operatør krever et brukernavn, så skriv det her. &lt;br&gt;Hvis ikke, så skriv hva som helst her.</translation>
    </message>
    <message>
        <source>if your operator requires a password, fill it in here.&lt;br&gt;If not, you probably want to put something dummy in here.</source>
        <translation>hvis din operatør krever et passord, så skriv det her. &lt;br&gt;Hvis ikke, så skriv hva som helst her.</translation>
    </message>
    <message>
        <source>Save the changes to the profile.&lt;br&gt;Only enabled if there are changes &lt;b&gt;and&lt;/b&gt; you have entered a valid APN.</source>
        <translation>Lagre endringene til profilen. &lt;br&gt; Dette skjer bare om du har endret på noe &lt;b&gt;og&lt;/b&gt; har skrevet inn en gyldig APN.</translation>
    </message>
    <message>
        <source>If your subscription is time based,&lt;br&gt;enter the number of minutes whilst roaming per month here.</source>
        <translation>Hvis din kontrakt er basert på en tidskvote, &lt;br&gt;så skriv inn minuttene per måned i kvoten hvor du roamer.</translation>
    </message>
    <message>
        <source>If your subscription is time based,&lt;br&gt;enter the number of minutes for your home network per month here.</source>
        <translation>Hvis din kontrakt er basert på en tidskvote, &lt;br&gt;så skriv inn minuttene per måned i kvoten hvor du er på nettet til din operatør.</translation>
    </message>
    <message>
        <source>Time:</source>
        <translation>Tid:</translation>
    </message>
    <message>
        <source>Traffic:</source>
        <translation>Trafikk:</translation>
    </message>
    <message>
        <source>empty user/pass</source>
        <translation>tomt brukernavn/passord</translation>
    </message>
    <message>
        <source>Are you sure you want to leave username or password empty?&lt;br&gt;If you do not specify a username and/or password, connection with the operator may not work as expected.&lt;br&gt;Most operators expect dummy (but non-empty) values here.</source>
        <translation>Er du sikker på at du vil la feltene for brukernavn og passord være tomme?&lt;br&gt;Hvis du ikke skriver inn brukernavn og/eller passord, så kan det hende at oppkoblingen ikke virker som den skal.&lt;br&gt;De fleste operatører forventer et eller annet skrevet inn her.</translation>
    </message>
    <message>
        <source>Your APN contains illegal characters.
You cannot save your profile until you fix this.</source>
        <translation>Din APN inneholder ugyldige tegn.
Du kan ikke lagre din profil før du har ordnet dette.</translation>
    </message>
</context>
<context>
    <name>smsdialog</name>
    <message>
        <source>Send SMS</source>
        <translation>Send SMS</translation>
    </message>
    <message>
        <source>Send SMS to number:</source>
        <translation>Send SMS til nummer:</translation>
    </message>
    <message>
        <source>enter the number to SMS to here</source>
        <translation>skriv inn nummeret SMS skal sendes til</translation>
    </message>
    <message>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <source>The number of characters in your message</source>
        <translation>Antall tegn i din melding</translation>
    </message>
    <message>
        <source>/</source>
        <translation>/</translation>
    </message>
    <message>
        <source>162</source>
        <translation>162</translation>
    </message>
    <message>
        <source>The number of characters left to send in this message</source>
        <translation>Antall ubrukte tegn for denne meldingen</translation>
    </message>
    <message>
        <source>Characters typed / left:</source>
        <translation>Tegn skrevet: / Tegn igjen:</translation>
    </message>
    <message>
        <source>Message:</source>
        <translation>Melding:</translation>
    </message>
    <message>
        <source>enter your SMS message here, max 162 characters</source>
        <translation>skriv inn din SMS melding her, maks 162 tegn</translation>
    </message>
    <message>
        <source>Clea&amp;r</source>
        <translation>S&amp;lett</translation>
    </message>
    <message>
        <source>&amp;Send</source>
        <translation>&amp;Send</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>sending SMS...</source>
        <translation>sender SMS ...</translation>
    </message>
    <message>
        <source>SMS sent (according to the device).</source>
        <translation>SMS sendt (i følge kortet).</translation>
    </message>
    <message>
        <source>SMS was not sent!</source>
        <translation>SMS ble ikke sendt!</translation>
    </message>
</context>
</TS>
