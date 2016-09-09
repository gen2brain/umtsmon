<!DOCTYPE TS><TS>
<defaultcodec></defaultcodec>
<context>
    <name>@default</name>
    <message>
        <source>not pending for any password.</source>
        <translation>no se espera ninguna contraseña</translation>
    </message>
    <message>
        <source>waiting for the PIN code for the SIM card.</source>
        <translation>esperando al código PIN de la tarjeta SIM</translation>
    </message>
    <message>
        <source>waiting for the PUK code to unblock the SIM card again.</source>
        <translation>esperando al código PUK para desbloquear la tarjeta SIM</translation>
    </message>
    <message>
        <source>waiting phone-to-SIM card password to be given.</source>
        <translation>esperando que se proporcione la contraseña de teléfono a tarjeta SIM</translation>
    </message>
    <message>
        <source>waiting phone-to-very SIM card password to be given.</source>
        <translation>esperando la contraseña  para la tarjeta SIM</translation>
    </message>
    <message>
        <source>waiting phone-to-very first SIM card password to be given.</source>
        <translation>esperando la contraseña para la primera tarjeta SIM.</translation>
    </message>
    <message>
        <source>waiting phone-to-very first SIM card unblocking password to be given.</source>
        <translation>esperando la contraseña de desbloqueo para la primera tarjeta SIM.</translation>
    </message>
    <message>
        <source>waiting SIM PIN2 to be given.</source>
        <translation>esperando all PIN2 de la SIM</translation>
    </message>
    <message>
        <source>waiting SIM PUK2 to be given.</source>
        <translation>esperando al PUK2 de la SIM</translation>
    </message>
    <message>
        <source>waiting network personalisation password to be given.</source>
        <translation>esperando a la contraseña de personalización de red</translation>
    </message>
    <message>
        <source>waiting network personalisation unblocking password to be given.</source>
        <translation>esperando contraseña de desbloqueo de la personalización de red</translation>
    </message>
    <message>
        <source>waiting network subset personalisation password to be given.</source>
        <translation>esperando la contraseña para personalización de subconjunt de red</translation>
    </message>
    <message>
        <source>waiting network subset personalisation unblock code to be given.</source>
        <translation>esperando código de desbloqueo para personalización de subconjunto de red</translation>
    </message>
    <message>
        <source>unknown (PH-SP PIN) - please contact author</source>
        <translation>desconocido (PH-SP PIN) - por favor, contacte con el autor</translation>
    </message>
    <message>
        <source>unknown (PH-SP PUK) - please contact author</source>
        <translation>desconocido (PH-SP PUK) - por favor, contacte con el autor</translation>
    </message>
    <message>
        <source>unknown (PH-CORP PIN) - please contact author</source>
        <translation>desonocido (PH-CORP PIN) - por favor, contacte con el autor</translation>
    </message>
    <message>
        <source>unknown (PH-CORP PUK) - please contact author</source>
        <translation>desconocido (PH-CORP PUK) - por favor, contacte con el autor</translation>
    </message>
</context>
<context>
    <name>AboutDialogUmtsmon</name>
    <message>
        <source>About</source>
        <translation>Acerca de</translation>
    </message>
    <message>
        <source>&lt;b&gt;&lt;font size=&quot;+2&quot;&gt;UMTSmon&lt;/font&gt; - Monitoring your UMTS PCMCIA card&lt;/b&gt;&lt;br&gt;&lt;br&gt;
(C) 2006,2007,2008 by Klaas van Gend and others.&lt;br&gt;&lt;br&gt;

&lt;b&gt;This application is licensed by the GPL v2 license.&lt;/b&gt;&lt;br&gt;
The COPYING tab explains the rights and constraints that are granted to you due to this software license.
&lt;br&gt;
Refer to the AUTHORS tab to find out what parts of this application were derived/taken from other sources.</source>
        <translation>&lt;b&gt;&lt;font size=&quot;+2&quot;&gt;UMTSmon&lt;/font&gt; - Monitorizando su tarjeta PCMCIA UMTS&lt;/b&gt;&lt;br&gt;&lt;br&gt;
(C) 2006,2007,2008 Klaas van Gend y otros.&lt;br&gt;&lt;br&gt;
&lt;b&gt;Esta aplicación tiene licencia GPL v2.&lt;/b&gt;&lt;br&gt;
La pestaña LICENCIA  explica los derechos y limitaciones que se le conceden por esa licencia de software.
&lt;br&gt;
Acuda a la pestaña AUTORES para encontrar qué partes de esta aplicación se derivan/toman otras fuentes.</translation>
    </message>
    <message>
        <source>version updated at runtime</source>
        <translation>versión actualizada en tiempo de ejecución</translation>
    </message>
    <message>
        <source>COPYING</source>
        <translation>LICENCIA</translation>
    </message>
    <message>
        <source>AUTHORS</source>
        <translation>AUTORES</translation>
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
AUTORES DE UMTSMON(new line)
==================(new line)
(new line)
umtsmon fue escrito por  Klaas van Gend y Christofer Wesseling durante 2006, 2007 and 2008.
Está licenciado bajo la Licencia GPLv2, vease el fichero COPIANDO

Sin embargo, Klaas no es el único que posee el copyright ya que algunas partes se derivan
de o están basados en otros trabajos. Además algunas personas dieron consejos en varios aspectos
de la implementación. Este fichero intenta enumerar esas contribuciones:



1)
la clase SerialPort está basada en COMGT 0.3 por Paul Hardwick.
COMGT a su vez estuvo basada en un paquete llamado dcon por Daniel Chouinard
Vease http://www.pharscape.org/content/view/46/70/
COMGT v0.3 se licenció bajo GPLv2 o posterior.

Paul Hardwick también contribuyó a las clases DetectCard y Device.


2)
La depuración de algunas cuestiones de la clase SerialPort se realizó con sugerencias de:
* Roel van den Broeck
* Martijn Wijms
* Tom Geelen
* Wouter Moors

3)
La imagen ok.png está tomada del fichero kdelibs3.rpm de SuSE9.3
Pertenece al conjunto de iconos CrystalSVGt.Según ese fichero, todos los ficheros
están licenciados a través de la GPL o LGPL.

4)
El logo UMTSmon fue dibujado por &apos;Tazz_tux&apos;.

5)
La Autodetección para los drivers de Nozomi 2.2+  y la enumeración múltiple de sockets pcmcia
fue una contribución de Stefan Seyfried [seife@suse.de]
Stefan también trabajo otros aspectos relativos a la detección de tarjetas.

6)
El código &apos;Runner&apos; para ejecutar otros programas desde dentro de umtsmon, específicamente la parte para comunicar a través
de tuberías (pipes) se inspiró en el código mostrado en:
http://www.cryptnet.net/fdp/gtk/exec.html
el código en esta página está protegido por GPL y tiene el copyright V. Alex Brennen [vab@metanet.org] .

7)
Los iconos en los directorios: images/{16|22|SVG} pertenecen al conjunto de iconos Tango,
disponibles en: http://tango.freedesktop.org/Tango_Icon_Library
Estos iconos están licenciados bajo la licencia &quot;Creative Commons Attribution Share-Alike&quot;
Nosotros pensamos que cumplimos con los requerimientos de la licencia, incluso a pesar que 
distribuimos los iconos incluídos en el binario de umtsmon.

8)
Varios problemas relacionados con la compilación y el interface de usuario fueron
corregidos por Gerald Pfeifer.

9)
Parche para la detección del Vodafone 3G por dj9pz (Arnd Gronenberg)

10)
Correcciones para Huawei E220 por Norbert Preining (norbusan)

11)
Lubomir Schmidt comenzó la traducción al alemán y realizó muchos tests.

12)
traducción adicional al alemán por Philipp Kießler and Dietmar Maurer.

13)
traducción al portugués (pt_PT) Pedro Algarvio.

14)
algunas correcciones por Carsten Clasohm.

15)
varias mejoras al interface de usuario por Danny Kukawka.

16) 
traducción al español (de España) es_ES por Emilio Arrufat


Si encuentra alguna omisión, por favor contacte con el autor a través del sitio web de Sourceforge:
http://www.sourceforge.net/projects/umtsmon

</translation>
    </message>
    <message>
        <source>
<byte value="x9"/><byte value="x9"/>    GNU GENERAL PUBLIC LICENSE
<byte value="x9"/><byte value="x9"/>       Version 2, June 1991

 Copyright (C) 1989, 1991 Free Software Foundation, Inc.
                       51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

<byte value="x9"/><byte value="x9"/><byte value="x9"/>    Preamble

  The licenses for most software are designed to take away your
freedom to share and change it.  By contrast, the GNU General Public
License is intended to guarantee your freedom to share and change free
software--to make sure the software is free for all its users.  This
General Public License applies to most of the Free Software
Foundation&apos;s software and to any other program whose authors commit to
using it.  (Some other Free Software Foundation software is covered by
the GNU Library General Public License instead.)  You can apply it to
your programs, too.

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
this service if you wish), that you receive source code or can get it
if you want it, that you can change the software or use pieces of it
in new free programs; and that you know you can do these things.

  To protect your rights, we need to make restrictions that forbid
anyone to deny you these rights or to ask you to surrender the rights.
These restrictions translate to certain responsibilities for you if you
distribute copies of the software, or if you modify it.

  For example, if you distribute copies of such a program, whether
gratis or for a fee, you must give the recipients all the rights that
you have.  You must make sure that they, too, receive or can get the
source code.  And you must show them these terms so they know their
rights.

  We protect your rights with two steps: (1) copyright the software, and
(2) offer you this license which gives you legal permission to copy,
distribute and/or modify the software.

  Also, for each author&apos;s protection and ours, we want to make certain
that everyone understands that there is no warranty for this free
software.  If the software is modified by someone else and passed on, we
want its recipients to know that what they have is not the original, so
that any problems introduced by others will not reflect on the original
authors&apos; reputations.

  Finally, any free program is threatened constantly by software
patents.  We wish to avoid the danger that redistributors of a free
program will individually obtain patent licenses, in effect making the
program proprietary.  To prevent this, we have made it clear that any
patent must be licensed for everyone&apos;s free use or not licensed at all.

  The precise terms and conditions for copying, distribution and
modification follow.

<byte value="x9"/><byte value="x9"/>    GNU GENERAL PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. This License applies to any program or other work which contains
a notice placed by the copyright holder saying it may be distributed
under the terms of this General Public License.  The &quot;Program&quot;, below,
refers to any such program or work, and a &quot;work based on the Program&quot;
means either the Program or any derivative work under copyright law:
that is to say, a work containing the Program or a portion of it,
either verbatim or with modifications and/or translated into another
language.  (Hereinafter, translation is included without limitation in
the term &quot;modification&quot;.)  Each licensee is addressed as &quot;you&quot;.

Activities other than copying, distribution and modification are not
covered by this License; they are outside its scope.  The act of
running the Program is not restricted, and the output from the Program
is covered only if its contents constitute a work based on the
Program (independent of having been made by running the Program).
Whether that is true depends on what the Program does.

  1. You may copy and distribute verbatim copies of the Program&apos;s
source code as you receive it, in any medium, provided that you
conspicuously and appropriately publish on each copy an appropriate
copyright notice and disclaimer of warranty; keep intact all the
notices that refer to this License and to the absence of any warranty;
and give any other recipients of the Program a copy of this License
along with the Program.

You may charge a fee for the physical act of transferring a copy, and
you may at your option offer warranty protection in exchange for a fee.

  2. You may modify your copy or copies of the Program or any portion
of it, thus forming a work based on the Program, and copy and
distribute such modifications or work under the terms of Section 1
above, provided that you also meet all of these conditions:

    a) You must cause the modified files to carry prominent notices
    stating that you changed the files and the date of any change.

    b) You must cause any work that you distribute or publish, that in
    whole or in part contains or is derived from the Program or any
    part thereof, to be licensed as a whole at no charge to all third
    parties under the terms of this License.

    c) If the modified program normally reads commands interactively
    when run, you must cause it, when started running for such
    interactive use in the most ordinary way, to print or display an
    announcement including an appropriate copyright notice and a
    notice that there is no warranty (or else, saying that you provide
    a warranty) and that users may redistribute the program under
    these conditions, and telling the user how to view a copy of this
    License.  (Exception: if the Program itself is interactive but
    does not normally print such an announcement, your work based on
    the Program is not required to print an announcement.)

These requirements apply to the modified work as a whole.  If
identifiable sections of that work are not derived from the Program,
and can be reasonably considered independent and separate works in
themselves, then this License, and its terms, do not apply to those
sections when you distribute them as separate works.  But when you
distribute the same sections as part of a whole which is a work based
on the Program, the distribution of the whole must be on the terms of
this License, whose permissions for other licensees extend to the
entire whole, and thus to each and every part regardless of who wrote it.

Thus, it is not the intent of this section to claim rights or contest
your rights to work written entirely by you; rather, the intent is to
exercise the right to control the distribution of derivative or
collective works based on the Program.

In addition, mere aggregation of another work not based on the Program
with the Program (or with a work based on the Program) on a volume of
a storage or distribution medium does not bring the other work under
the scope of this License.

  3. You may copy and distribute the Program (or a work based on it,
under Section 2) in object code or executable form under the terms of
Sections 1 and 2 above provided that you also do one of the following:

    a) Accompany it with the complete corresponding machine-readable
    source code, which must be distributed under the terms of Sections
    1 and 2 above on a medium customarily used for software interchange; or,

    b) Accompany it with a written offer, valid for at least three
    years, to give any third party, for a charge no more than your
    cost of physically performing source distribution, a complete
    machine-readable copy of the corresponding source code, to be
    distributed under the terms of Sections 1 and 2 above on a medium
    customarily used for software interchange; or,

    c) Accompany it with the information you received as to the offer
    to distribute corresponding source code.  (This alternative is
    allowed only for noncommercial distribution and only if you
    received the program in object code or executable form with such
    an offer, in accord with Subsection b above.)

The source code for a work means the preferred form of the work for
making modifications to it.  For an executable work, complete source
code means all the source code for all modules it contains, plus any
associated interface definition files, plus the scripts used to
control compilation and installation of the executable.  However, as a
special exception, the source code distributed need not include
anything that is normally distributed (in either source or binary
form) with the major components (compiler, kernel, and so on) of the
operating system on which the executable runs, unless that component
itself accompanies the executable.

If distribution of executable or object code is made by offering
access to copy from a designated place, then offering equivalent
access to copy the source code from the same place counts as
distribution of the source code, even though third parties are not
compelled to copy the source along with the object code.

  4. You may not copy, modify, sublicense, or distribute the Program
except as expressly provided under this License.  Any attempt
otherwise to copy, modify, sublicense or distribute the Program is
void, and will automatically terminate your rights under this License.
However, parties who have received copies, or rights, from you under
this License will not have their licenses terminated so long as such
parties remain in full compliance.

  5. You are not required to accept this License, since you have not
signed it.  However, nothing else grants you permission to modify or
distribute the Program or its derivative works.  These actions are
prohibited by law if you do not accept this License.  Therefore, by
modifying or distributing the Program (or any work based on the
Program), you indicate your acceptance of this License to do so, and
all its terms and conditions for copying, distributing or modifying
the Program or works based on it.

  6. Each time you redistribute the Program (or any work based on the
Program), the recipient automatically receives a license from the
original licensor to copy, distribute or modify the Program subject to
these terms and conditions.  You may not impose any further
restrictions on the recipients&apos; exercise of the rights granted herein.
You are not responsible for enforcing compliance by third parties to
this License.

  7. If, as a consequence of a court judgment or allegation of patent
infringement or for any other reason (not limited to patent issues),
conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot
distribute so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you
may not distribute the Program at all.  For example, if a patent
license would not permit royalty-free redistribution of the Program by
all those who receive copies directly or indirectly through you, then
the only way you could satisfy both it and this License would be to
refrain entirely from distribution of the Program.

If any portion of this section is held invalid or unenforceable under
any particular circumstance, the balance of the section is intended to
apply and the section as a whole is intended to apply in other
circumstances.

It is not the purpose of this section to induce you to infringe any
patents or other property right claims or to contest validity of any
such claims; this section has the sole purpose of protecting the
integrity of the free software distribution system, which is
implemented by public license practices.  Many people have made
generous contributions to the wide range of software distributed
through that system in reliance on consistent application of that
system; it is up to the author/donor to decide if he or she is willing
to distribute software through any other system and a licensee cannot
impose that choice.

This section is intended to make thoroughly clear what is believed to
be a consequence of the rest of this License.

  8. If the distribution and/or use of the Program is restricted in
certain countries either by patents or by copyrighted interfaces, the
original copyright holder who places the Program under this License
may add an explicit geographical distribution limitation excluding
those countries, so that distribution is permitted only in or among
countries not thus excluded.  In such case, this License incorporates
the limitation as if written in the body of this License.

  9. The Free Software Foundation may publish revised and/or new versions
of the General Public License from time to time.  Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns.

Each version is given a distinguishing version number.  If the Program
specifies a version number of this License which applies to it and &quot;any
later version&quot;, you have the option of following the terms and conditions
either of that version or of any later version published by the Free
Software Foundation.  If the Program does not specify a version number of
this License, you may choose any version ever published by the Free Software
Foundation.

  10. If you wish to incorporate parts of the Program into other free
programs whose distribution conditions are different, write to the author
to ask for permission.  For software which is copyrighted by the Free
Software Foundation, write to the Free Software Foundation; we sometimes
make exceptions for this.  Our decision will be guided by the two goals
of preserving the free status of all derivatives of our free software and
of promoting the sharing and reuse of software generally.

<byte value="x9"/><byte value="x9"/><byte value="x9"/>    NO WARRANTY

  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
PROVIDE THE PROGRAM &quot;AS IS&quot; WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.

  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.

<byte value="x9"/><byte value="x9"/>     END OF TERMS AND CONDITIONS
                            </source>
        <translation></translation>
    </message>
    <message encoding="UTF-8">
        <source>
AUTHORS OF UMTSMON
==================

Over 90% of umtsmon was written by Klaas van Gend in 2006, 2007, 2008 and 2009.
It is licensed under the GPLv2 License, see the file COPYING

However, Klaas is not the only copyright holder as some parts were derived
from or based on other sources. Also: several people gave advise on some 
implementation topics or did supply patches or translations. 
This file tries to list those contributions:

0)
Christofer Wesseling contributed to several parts of umtsmon as part 
of his internship work at T-Mobile.

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
The UMTSmon logo as used on the website was drawn by &apos;Tazz_tux&apos;.

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
additional German translations by Philipp Kießler, Dietmar Maurer and Thorsten Hesemeyer.

13)
translation work into Portugese (pt_PT) by Pedro Algarvio.

14)
some fixes by Carsten Clasohm.

15)
several UI improvements by Danny Kukawka. 

16)
Translation into Norwegian Bokmaal by Klaus Ade Johnstad.

17)
comprehensive Sierra Wireless device support added by Phil Gorsuch

18)
Translation into Spanish by Emilio Arrufat, enhancements by Manuel Tornos

19)
Translation into Italian by Alessandro Briosi

20)
Translation into Polish by Tomasz Dominikowski

21)
Translation into Indonesian by Andika Triwidada

22)
The CID is now configurable from the config file thanks to Tobias Stober

23)
Michael Gisbers helped getting the HSO devices to work

24)
Till Busch provided tricks to add a &quot;make install&quot; target to umtsmon.

25)
Robert Verspuy fixed an issue with the Samsung Innov8 or i8510 mobile phone

26)
Martin Dummer provided the manpage and several fixes to umtsmon to allow
adding umtsmon to Debian.

If you have found any ommissions, please contact the author through the Sourceforge website:
http://www.sourceforge.net/projects/umtsmon
                            </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EnterPIN</name>
    <message>
        <source>Enter PIN</source>
        <translation>Introduca el PIN</translation>
    </message>
    <message>
        <source>Info</source>
        <translation>Información</translation>
    </message>
    <message>
        <source>Please enter the PIN of your 
SIM card</source>
        <translation>Por favor, introduzca el PIN de
su tarjeta SIM</translation>
    </message>
    <message>
        <source>PIN</source>
        <translation>PIN</translation>
    </message>
    <message>
        <source>Enter the PIN of your SIM-Card.</source>
        <translation>Introduzca el PIN de su tarjeta SIM</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>This is not a valid PIN code.</source>
        <translation>Código PIN no válido</translation>
    </message>
</context>
<context>
    <name>EnterPUK</name>
    <message>
        <source>Please enter your PUK code</source>
        <translation>Introduzca el código PUK</translation>
    </message>
    <message>
        <source>PUK of your SIM card</source>
        <translation>PUK de su tarjeta SIM</translation>
    </message>
    <message>
        <source>Please enter the PUK-Code of your SIM card</source>
        <translation>Por favor, introduzca el código PUK de su tarjeta SIM</translation>
    </message>
    <message>
        <source>Enter here the PUK of your SIM card</source>
        <translation>Introduzca el PUK de su tarjeta SIM</translation>
    </message>
    <message>
        <source>The new PIN</source>
        <translation>Nuevo PIN</translation>
    </message>
    <message>
        <source>Enter here the new PIN for your SIM card</source>
        <translation>Introduzca el nuevo PIN de la tarjeta SIM</translation>
    </message>
    <message>
        <source>Enter here the new PIN of your SIM card</source>
        <translation>Introduzca aquí el nuevo PIN de su tarjeta SIM</translation>
    </message>
    <message>
        <source>Info</source>
        <translation>Información</translation>
    </message>
    <message>
        <source>You have entered a wrong PIN  3 times
Please Enter the PUK-Code.</source>
        <translation>Ha introducido erróneamente el código PIN(sp)(sp)3 veces
Por favor, introduzca el código PUK.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>PIN or PUK was not valid!</source>
        <translation>¡El PIN o el PUK no son válidos!</translation>
    </message>
    <message>
        <source>PIN needed after PUK?
That&apos;s unexpected!
Please mail info to: umtsmon-develop@sourceforge.net</source>
        <translation>¿Se necesita el PIN despúes del PUK?
¡Esto es inesperado!
Por favor envía información a: umtsmon-develop@sourceforge.net</translation>
    </message>
    <message>
        <source>success :-)</source>
        <translation>éxito :-)</translation>
    </message>
    <message>
        <source>PUK was not correct.</source>
        <translation>PUK correcto.</translation>
    </message>
    <message>
        <source>Something went wrong in the PIN procedures
umtsmon cannot recover
Please try a restart and/or
insert the SIM card in a GSM phone</source>
        <translation>Algo fue mal en los procedimientos sobre el PIN
umtsmon no puede continuar
Por favor, intenta reiniciar y/o
inserta la tarjeta SIM en un teléfono GSM</translation>
    </message>
</context>
<context>
    <name>InfoDialogUmtsmon</name>
    <message>
        <source>Info</source>
        <translation type="obsolete">Información</translation>
    </message>
    <message>
        <source>SIM / Device</source>
        <translation>SIM / Dispositivo</translation>
    </message>
    <message>
        <source>IMSI</source>
        <translation>IMSI</translation>
    </message>
    <message>
        <source> - no Value -</source>
        <translation>- sin Valor - </translation>
    </message>
    <message>
        <source>Manufacturer</source>
        <translation>Fabricante</translation>
    </message>
    <message>
        <source>Card-Type</source>
        <translation>Tipo de Tarjeta</translation>
    </message>
    <message>
        <source>IMEI</source>
        <translation>IMEI</translation>
    </message>
    <message>
        <source>Revision</source>
        <translation>Revisión</translation>
    </message>
    <message>
        <source>Linux Kernel</source>
        <translation>Kernel de Linux</translation>
    </message>
    <message>
        <source>Version</source>
        <translation>Versión</translation>
    </message>
    <message>
        <source>no information available</source>
        <translation>no hay información disponible</translation>
    </message>
    <message>
        <source>modules</source>
        <translation>módulos</translation>
    </message>
    <message>
        <source>module</source>
        <translation>módulo</translation>
    </message>
    <message>
        <source>size</source>
        <translation>tamaño</translation>
    </message>
    <message>
        <source>use count</source>
        <translation>contador de uso</translation>
    </message>
    <message>
        <source>devices</source>
        <translation>dispositivos</translation>
    </message>
    <message>
        <source>major nr</source>
        <translation>núm. mayor</translation>
    </message>
    <message>
        <source>device</source>
        <translation>dispositivo</translation>
    </message>
    <message>
        <source>System</source>
        <translation>Sistema</translation>
    </message>
    <message>
        <source>pppd version:</source>
        <translation>versión de pppd:</translation>
    </message>
    <message>
        <source>distro release:</source>
        <translation>distribución:</translation>
    </message>
    <message>
        <source>- no Value -</source>
        <translation>- sin Valor -</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
    <message>
        <source>Fetching device informations...</source>
        <translation type="obsolete">Obteniendo datos de dispositivo...</translation>
    </message>
    <message>
        <source>No answer from device.</source>
        <translation>No hay respuesta del dispositivo.</translation>
    </message>
    <message>
        <source>System Info</source>
        <translation>Información del Sistema</translation>
    </message>
    <message>
        <source>AT port</source>
        <translation>Puerto AT</translation>
    </message>
    <message>
        <source>umtsmon driver name</source>
        <translation>Driver usado por umtsmon</translation>
    </message>
    <message>
        <source>PPP port</source>
        <translation>Puerto PPP</translation>
    </message>
    <message>
        <source>Fetching device information...</source>
        <translation>Obteniendo datos de dispositivo...</translation>
    </message>
</context>
<context>
    <name>LCDDisplay</name>
    <message>
        <source>n/a</source>
        <translation>n/d</translation>
    </message>
    <message>
        <source>%1d %2h</source>
        <translation>%1d %2h</translation>
    </message>
    <message>
        <source>%1h %2m</source>
        <translation>%1h %2m</translation>
    </message>
    <message>
        <source>%1m %2s</source>
        <translation>%1m %2s</translation>
    </message>
</context>
<context>
    <name>ManageProfiles</name>
    <message>
        <source>Manage Profiles</source>
        <translation>Gestión de Perfiles</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>This area shows the profiles that you can use to connect with</source>
        <translation>Esta área muestra los perfiles de conexión</translation>
    </message>
    <message>
        <source>&amp;Add profile...</source>
        <translation>&amp;Añadir perfil</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>Create another profile</source>
        <translation>Crear otro perfil</translation>
    </message>
    <message>
        <source>&amp;Edit profile...</source>
        <translation>&amp;Editar perfil</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>change an existing profile</source>
        <translation>modificar perfil existente</translation>
    </message>
    <message>
        <source>&amp;Set as active</source>
        <translation>&amp;Marcar como activo</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+M</translation>
    </message>
    <message>
        <source>make the selected profile active</source>
        <translation>marcar como activo el perfil seleccionado</translation>
    </message>
    <message>
        <source>&amp;Delete profile</source>
        <translation>&amp;Borrar perfil</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>delete the currently selected profile</source>
        <translation>borrar el perfil seleccionado</translation>
    </message>
    <message>
        <source>please_replace_me</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NetworkOperatorWizard</name>
    <message>
        <source>Network Operator Wizard</source>
        <translation>Asistente para Operadora de Red</translation>
    </message>
    <message>
        <source>Page 1: Welcome</source>
        <translation>Página 1: Bienvenida</translation>
    </message>
    <message>
        <source>&lt;b&gt;&lt;p align=&quot;center&quot;&gt;Welcome to the&lt;br&gt; 
Network Operator Wizard&lt;/p&gt;&lt;/b&gt;
&lt;p&gt;This wizard will help you choose an operator.&lt;/p&gt;
&lt;p&gt;Please note: when abroad, usually all operators will accept
 your connection and charge you &quot;roaming fees&quot; - 
much more expensive.&lt;/P&gt;</source>
        <translation>&lt;b&gt;&lt;p align=&quot;center&quot;&gt;Bienvenido al&lt;br&gt;
Asistente para Operadora de Red&lt;/p&gt;&lt;/b&gt;
&lt;p&gt;Este asistente le ayudará a seleccionar una operadora.&lt;/p&gt;
&lt;p&gt;Por favor, tenga en cuenta que en el extranjero usualmente todas las operadoras
aceptarán su conexión y le aplicarán tarifas de &quot;roaming&quot; - mucho más caras.&lt;/P&gt;</translation>
    </message>
    <message>
        <source>Page 2: Who picks the operator?</source>
        <translation>Página 2: ¿Quién escoge la operadora?</translation>
    </message>
    <message>
        <source>who picks?</source>
        <translation>¿quién escoge?</translation>
    </message>
    <message>
        <source>have the card decide</source>
        <translation>dejar que el dispositivo decida</translation>
    </message>
    <message>
        <source>let me select an operator</source>
        <translation>permitirme seleccionar una operadora</translation>
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
        <translation>&lt;p&gt;Su  tarjeta SIM contiene
un conjunto de operadoras preferidas en el extranjero.
El dispositivo puede seleccionarlas automáticamente.
(&quot;Preferido&quot; no significa necesariamente de menor coste)&lt;/p&gt;

&lt;p&gt; Por favor, tenga en cuenta que si ese operador
preferido tiene mala cobertura, no obtendrá conexión
a menos que lo seleccione manualmente.&lt;/p&gt;

&lt;p&gt; Sin embargo, si la selección es manuel, el dispositivo
no cambiará de red a menos que Ud. se lo indique - viajar
a otro país requiere que se ejecute este asistente de nuevo.  &lt;/p&gt;</translation>
    </message>
    <message>
        <source>Page 3: Get network strengths?</source>
        <translation>Página 3: ¿Obtener intensidades de coberturas?</translation>
    </message>
    <message>
        <source>&lt;p&gt;After searching for operators, we will start getting signal strengths.&lt;/p&gt;

&lt;p&gt;This will however significantly increase the search time!
&lt;/p&gt;</source>
        <translation>&lt;p&gt;Despúes de buscar operadoras. empezaremos a obtener indicadores de coberturas. &lt;/p&gt;

&lt;p&gt;Esto sin embargo incrementará el tiempo de búsqueda.
&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Don&apos;t get signal strengths</source>
        <translation>No obtener intensidad de cobertura</translation>
    </message>
    <message>
        <source>&lt;p align=&quot;center&quot;&gt;retrieving signal strengths not implemented yet&lt;/p&gt;</source>
        <translation>&lt;p align=&quot;center&quot;&gt;la obtención de cobertura no está implementada todavía&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Page 4: Searching for networks...</source>
        <translation>Página 4: Busqueda de redes...</translation>
    </message>
    <message>
        <source>&lt;p align=&quot;center&quot;&gt;Searching for networks...&lt;/p&gt;

&lt;p align=&quot;center&quot;&gt;Please wait...&lt;/p&gt;</source>
        <translation>&lt;p align=&quot;center&quot;&gt;Buscando redes...&lt;/p&gt;

&lt;p align=&quot;center&quot;&gt;Por favor, espere...&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Page 5: Select a Network Operator</source>
        <translation>Página 5: Selección de Operadora de Red</translation>
    </message>
    <message>
        <source>Please select your network:</source>
        <translation>Por favor, seleccione su red:</translation>
    </message>
    <message>
        <source>Still getting network strengths...</source>
        <translation>Todavía obteniendo intensidad de cobertura...</translation>
    </message>
    <message>
        <source>Operator</source>
        <translation>Operadora</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Signal</source>
        <translation>Cobertura</translation>
    </message>
    <message>
        <source>ID</source>
        <translation>Id</translation>
    </message>
    <message>
        <source>This table lists the operators to choose from.</source>
        <translation>Esta tabla lista las operadoras a escoger.</translation>
    </message>
    <message>
        <source>Page 6: Connecting to operator...</source>
        <translation>Página 6: Conectando a la operadora...</translation>
    </message>
    <message>
        <source>&lt;p align=&quot;center&quot;&gt;Connecting to your network...&lt;/p&gt;

&lt;p align=&quot;center&quot;&gt;Please wait...&lt;/p&gt;</source>
        <translation>&lt;p align=&quot;center&quot;&gt;Conectando a su red...&lt;/p&gt;

&lt;p align=&quot;center&quot;&gt;Por favor, espere...&lt;/p&gt;</translation>
    </message>
    <message>
        <source>The End</source>
        <translation>Fin</translation>
    </message>
    <message>
        <source>&lt;b&gt;&lt;p align=&quot;center&quot;&gt;Network Operator
Wizard&lt;br&gt;completed&lt;/p&gt;&lt;/b&gt;
&lt;br&gt;&lt;br&gt;
&lt;p&gt;Have fun on the internet!&lt;/P&gt;</source>
        <translation>&lt;b&gt;&lt;p align=&quot;center&quot;&gt;Asistente de Operadora de Red
&lt;br&gt;completado&lt;/p&gt;&lt;/b&gt;
&lt;br&gt;&lt;br&gt;
&lt;p&gt;¡Diviértete en Internet&lt;/P&gt;</translation>
    </message>
    <message>
        <source>No network operators were found.
You might want to try again.</source>
        <translation>No se encontró ninguna red.
Puede intentarlo de nuevo.</translation>
    </message>
</context>
<context>
    <name>NewProfileDialog</name>
    <message>
        <source>New Profile</source>
        <translation>Nuevo Perfil</translation>
    </message>
    <message>
        <source>Profile name</source>
        <translation>Nombre de perfil</translation>
    </message>
    <message>
        <source>please enter a new profile name</source>
        <translation>por favor, introduzca un nombre nuevo de perfil</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Crear</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>This profile name is invalid, please change.</source>
        <translation>Nombre de perfil incorrecto, por favor, cámbielo.</translation>
    </message>
</context>
<context>
    <name>PPPErrorLogsDialog</name>
    <message>
        <source>PPP Error Logs</source>
        <translation>Log de error de PPP</translation>
    </message>
    <message>
        <source>Problem</source>
        <translation>Problema</translation>
    </message>
    <message>
        <source>&lt;b&gt;Connection failed&lt;/b&gt;&lt;br&gt;&lt;br&gt;

Unfortunately, PPP could not make a connection.&lt;br&gt;&lt;br&gt;
On the next two tabs,  you&apos;ll find the information PPP returned. Please check if you have the right PPP options enabled. If you need more information, please re-run this program from the command line with the -v4 argument.</source>
        <translation>&lt;b&gt;Conexión fallida&lt;/b&gt;&lt;br&gt;&lt;br&gt;

Desafortunadamente, PPP no pudo establecer conexión&lt;br&gt;&lt;br&gt;
En las dos siguientes pestañas, encontrará la información que devolvió PPP. Por favor, compruebe que tiene activadas las opciones correctas para PPP. Si necesita más información, por favor ejecute este programa desde un terminal con la opción -v4.</translation>
    </message>
    <message>
        <source>PPP stdout</source>
        <translation>Stdout de PPP</translation>
    </message>
    <message>
        <source>Copy this information into e.g. an e-mail.</source>
        <translation>Copie estos datos en, p.ej., un e-mail</translation>
    </message>
    <message>
        <source>PPP stderr</source>
        <translation>Stderr de PPP</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
</context>
<context>
    <name>PopupWithMemory</name>
    <message>
        <source>MyDialog</source>
        <translation></translation>
    </message>
    <message>
        <source>textLabel1</source>
        <translation></translation>
    </message>
    <message>
        <source>Do not show this message again</source>
        <translation>No mostrar este mensaje de nuevo</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>&lt;p align=&quot;center&quot;&gt;&lt;b&gt;APN info&lt;/b&gt;&lt;/p&gt; &lt;p align=&quot;left&quot;&gt;For internet-connection, most providers accept an empty APN-Name.&lt;br&gt;So you&apos;ll &lt;b&gt;not&lt;/b&gt; have to fill in APN, username,password.&lt;/p&gt; &lt;p align=&quot;center&quot;&gt;If your provider requires an APN, you can find a list of those here:&lt;br&gt; &lt;b&gt;http://www.pinstack.com/carrier_settings_apn_gateway.html&lt;/b&gt;&lt;/p&gt; &lt;p align=&quot;left&quot;&gt;For Private APNs, please ask your provider for support!&lt;/p&gt;</source>
        <translation>&lt;p align=&quot;center&quot;&gt;&lt;b&gt;Información sobre APN&lt;/b&gt;&lt;/p&gt; &lt;p align=&quot;left&quot;&gt;Para conexiones a internet, la mayoría de los proveedores aceptan un nombre de APN vacío.&lt;br&gt;Así que &lt;b&gt;no&lt;/b&gt; está obligado a rellenar APN, usuario y contraseña.&lt;/p&gt; &lt;p align=&quot;center&quot;&gt;Si su proveedor requiere un APN, puede encontrar una lista de ellos aquí:&lt;br&gt; &lt;b&gt;http://www.pinstack.com/carrier_settings_apn_gateway.html&lt;/b&gt;&lt;/p&gt; &lt;p align=&quot;left&quot;&gt;Para datos de un APN privado, ¡solicite ayuda a su proveedor!&lt;/p&gt;</translation>
    </message>
    <message>
        <source>n/a</source>
        <translation>n/d</translation>
    </message>
    <message>
        <source>Could not write to device (anymore)</source>
        <translation>No se puede escribir en el dispositivo (nunca más)</translation>
    </message>
    <message>
        <source>Your device was recognised as a usbserial device,
yet the corresponding kernel module is not running on your system.

Do you want to load that module now?</source>
        <translation>Su dispositivo se reconoció como un dispositivo serie usb (usbserial)
pero el correspondiente módulo del kernel no está en ejecución.

¿Desea cargar el módulo ahora?</translation>
    </message>
    <message>
        <source>We think we found your device, but it shows too little ports.
This usually indicated that your device needs &apos;switching&apos;.
However, no switch application (usb_modeswitch) was found.
we will continue, but this might cause trouble later on...</source>
        <translation>Pensamos que hemos detectado su dispositivo, pero ofrece pocos puertos.
Esto normalmente indica que su dispositivo necesita &apos;conmutación&apos; (switching).
Sin embargo, no se encuentra ninguna aplicación de switch (usb_modeswitch);
podemos continuar, pero pueden surgir problemas más tarde...</translation>
    </message>
    <message>
        <source>umtsmon detected that you need to switch your device.
Click Yes if you agree.
 Note: this does not work properly for all devices yet!</source>
        <translation>umtsmon detectó que necesita conmutar (switch) su dispositivo.
Pulse Sí si está de acuerdo.
Nota: ¡no funciona correctamente en todos los dispositivos!, por el momento.</translation>
    </message>
    <message>
        <source>SIM card is moaning about something I don&apos;t know about:
</source>
        <translation type="obsolete">La tarjeta SIM está musitando algo que no puedo comprender:
</translation>
    </message>
    <message>
        <source>
Please insert the SIM into a regular phone and check it...</source>
        <translation type="obsolete">
Por favor, introduzca la tarjeta SIM en un teléfono y compruébela...</translation>
    </message>
    <message>
        <source>Unknown error - please review logs</source>
        <translation>Error desconocido - por favor, revisa los logs</translation>
    </message>
    <message>
        <source>Waiting for SIM-Interface...</source>
        <translation>Esperando al interface de SIM</translation>
    </message>
    <message>
        <source>SIM card handling resulted in this error:
      </source>
        <translation type="obsolete">El manejo de la tarjeta SIM acabó en este error:
      </translation>
    </message>
    <message>
        <source>Trying to set PIN/PUK</source>
        <translation>Intentando establecer PIN/PUK</translation>
    </message>
    <message>
        <source>Apparently there is already a pppd process running.
This prevents umtsmon from detecting your card,
umtsmon will now close.</source>
        <translation>Aparentemente ya hay un proceso ppp en ejecución.
Esto impide que umtsmon detecte su tarjeta,
umtsmon se cerrará.</translation>
    </message>
    <message>
        <source>No device could be found, the application will close.
If you need more info, please re-run from the command line.</source>
        <translation type="obsolete">No se detecta ningún dispositivo, se cerrará la aplicación.
Si necesita más información, por favor re-ejecute desde la línea de comandos.</translation>
    </message>
    <message>
        <source>Searching...</source>
        <translation>Buscando...</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation>Abortar</translation>
    </message>
    <message>
        <source>Welcome to umtsmon.

umtsmon will now search for a supported device.</source>
        <translation type="obsolete">Bienvenido a umtsmon.

umtsmon buscará ahora dispositivos soportados.</translation>
    </message>
    <message>
        <source>Your APN contains illegal characters.
Please go back to your Profile page and change the APN.</source>
        <translation>Su APN contiene caracteres ilegales.
Por favor vuelva a su página de perfiles y cambie el APN.</translation>
    </message>
    <message>
        <source>Warning: You did not specify an &apos;APN&apos; in your profile.
This may work ok, it may not - depending on your network operator.</source>
        <translation>Atención: No ha especificado un &apos;APN&apos; en su perfil.
Esto puede estar bien o no -dependiendo de su operador de red-.</translation>
    </message>
    <message>
        <source> message</source>
        <translation> mensaje</translation>
    </message>
    <message>
        <source>Welcome to umtsmon.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>umtsmon detected that you need to &apos;modeswitch&apos; your device.
However, you do not have any switch program installed.
We suggest you install usb_modeswitch and edit its config file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SIM card is moaning about something I don&apos;t know about:
&quot;%1&quot;
Please insert the SIM into a regular phone and check it...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SIM card handling resulted in this error:
  &quot;%1&quot;, sorry.
umtsmon will now close.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>!!! missing binary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 couldn&apos;t find binary %2, which is required for full operation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>user does not have write access</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>user does not have read access</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>read/write access is OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>user does not have read nor write access (or something else has a lock)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>not found on file system</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Modem Communication Ports</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You run as root - no need for groups</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Member of the following user groups</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>usually one needed to access the device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>is member</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>not a member - not fixable by umtsmon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>required if you want to make connections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>only required for PCMCIA cards</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>if this app is present, you probably need it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No Connection Class defined yet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>HSO device - no pppd required</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RadioPreferences</name>
    <message>
        <source>Radio Preferences</source>
        <translation>Preferencias de Radio</translation>
    </message>
    <message>
        <source>Limit radio types:</source>
        <translation>Limitar tipos de radio:</translation>
    </message>
    <message>
        <source>if your card supports this feature, you can limit the card to a certain radio technology. This is useful if you have a fringe coverage, e.g. the card is jumping between UMTS and GPRS because neither is really strong.</source>
        <translation>si su tarjeta lo soporta, puede limitarla a una cierta tecnología de radio. Esto es útil si se está en el límite de cobertura, esto es, si la tarjeta está conmutando de UMTS a GPRS porque ninguno se recibe realmente fuerte.</translation>
    </message>
    <message>
        <source>(changing radio types whilst a PPP connection exists will fail)</source>
        <translation>(cambiar el tipo de radio mientras hay una conexión PPP activa fallará)</translation>
    </message>
    <message>
        <source>automatic update of signal strength and download statistics</source>
        <translation type="obsolete">actualización automática de la intensidad de señal y estadisticas de tráfico</translation>
    </message>
    <message>
        <source>disable this if you don&apos;t want umtsmon to check the signal strength and the byte counts every three seconds</source>
        <translation type="obsolete">desactive si no desea que umtsmon compruebe la intensidad de señal y el contador de bytes cada tres segundos</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Failed to set your radio technology preference.</source>
        <translation>Falló la selección de teconología de radio</translation>
    </message>
</context>
<context>
    <name>SetLanguageDialog</name>
    <message>
        <source>Set Language</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Please select your language. 
This setting will be active after a restart.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available languages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Selecting the language failed ?!?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UIState</name>
    <message>
        <source>No device... (%1)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Detecting device %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SIM check %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Registering %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Connecting %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disconnecting %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>helperlistgui</name>
    <message>
        <source>Setup external applications</source>
        <translation>Configurar aplicaciones externas</translation>
    </message>
    <message>
        <source>Several helper applications and file permission settings are needed for proper function.
 Below is a list of those which also specifies whether everything is ok.  If is not, &quot;Fix&quot; will try to change the appropriate setting.</source>
        <translation>Para un correcto funcionamiento se necesitan varias aplicaciones de ayuda y permisos sobre ficheros.
Abajo hay una lista que también indica si todo está correcto.  Si no, &quot;Corregir&quot; intentará cambiar la configuración adecuada.</translation>
    </message>
    <message>
        <source>#</source>
        <translation>Num</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Directorio</translation>
    </message>
    <message>
        <source>Version</source>
        <translation>Versión</translation>
    </message>
    <message>
        <source>suid?</source>
        <translation>¿suid?</translation>
    </message>
    <message>
        <source>Remarks</source>
        <translation>Notas</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Ayuda</translation>
    </message>
    <message>
        <source>F1</source>
        <translation>F1</translation>
    </message>
    <message>
        <source>&amp;Fix</source>
        <translation>&amp;Corregir</translation>
    </message>
    <message>
        <source>Alt+F</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorar</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>don&apos;t adjust anything and continue running</source>
        <translation>no ajustar nada y continuar</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Salir</translation>
    </message>
    <message>
        <source>Alt+Q</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>stop running the application immediately</source>
        <translation>salir de la aplicación inmediatamente</translation>
    </message>
</context>
<context>
    <name>mainwindow</name>
    <message>
        <source>umtsmon mainwindow</source>
        <translation>ventana principal de umstmon</translation>
    </message>
    <message>
        <source>&amp;Connection</source>
        <translation>&amp;Conexión</translation>
    </message>
    <message>
        <source>&amp;SMS</source>
        <translation>&amp;SMS</translation>
    </message>
    <message>
        <source>&amp;PIN-Settings</source>
        <translation type="obsolete">Configuración de &amp;PIN</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Ayuda</translation>
    </message>
    <message>
        <source>Toolbar</source>
        <translation>Barra de Herramientas</translation>
    </message>
    <message>
        <source>manage profiles</source>
        <translation>gestión de perfiles</translation>
    </message>
    <message>
        <source>Connect with default Profile</source>
        <translation>Conectar con el perfil activo</translation>
    </message>
    <message>
        <source>disconnect</source>
        <translation>desconectar</translation>
    </message>
    <message>
        <source>create a new sms</source>
        <translation>nuevo SMS</translation>
    </message>
    <message>
        <source>pops up a dialog with system information.</source>
        <translation>muestra una ventana con información del sistema.</translation>
    </message>
    <message>
        <source>refresh operator/signal/radio/statistics on the display</source>
        <translation type="obsolete">refrescar la ventana con operador/señal/cobertura/estadisticas</translation>
    </message>
    <message>
        <source>Connect</source>
        <translation>Conectar</translation>
    </message>
    <message>
        <source>&amp;Connect</source>
        <translation>&amp;Conectar</translation>
    </message>
    <message>
        <source>Ctrl+C</source>
        <translation>Ctrl+C</translation>
    </message>
    <message>
        <source>Disconnect</source>
        <translation>Desconectar</translation>
    </message>
    <message>
        <source>&amp;Disconnect</source>
        <translation>&amp;Desconectar</translation>
    </message>
    <message>
        <source>Ctrl+D</source>
        <translation>Ctrl+D</translation>
    </message>
    <message>
        <source>Power down card</source>
        <translation>Apagar tarjeta</translation>
    </message>
    <message>
        <source>P&amp;ower down card</source>
        <translation>A&amp;pagar tarjeta</translation>
    </message>
    <message>
        <source>Reset card</source>
        <translation>Resetear tarjeta</translation>
    </message>
    <message>
        <source>R&amp;eset card</source>
        <translation>R&amp;esetear tarjeta</translation>
    </message>
    <message>
        <source>New SMS</source>
        <translation>Nuevo SMS</translation>
    </message>
    <message>
        <source>&amp;New SMS...</source>
        <translation>&amp;Nuevo SMS...</translation>
    </message>
    <message>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <source>Enable PIN</source>
        <translation>Activar PIN</translation>
    </message>
    <message>
        <source>&amp;Enable PIN</source>
        <translation type="obsolete">&amp;Activar PIN</translation>
    </message>
    <message>
        <source>&amp;Disable PIN</source>
        <translation type="obsolete">&amp;Desactivar PIN</translation>
    </message>
    <message>
        <source>Disable PIN</source>
        <translation>Desactivar PIN</translation>
    </message>
    <message>
        <source>Change PIN</source>
        <translation>Cambiar PIN</translation>
    </message>
    <message>
        <source>&amp;Change PIN</source>
        <translation type="obsolete">&amp;Cambiar PIN</translation>
    </message>
    <message>
        <source>System Info</source>
        <translation>Información del Sistema</translation>
    </message>
    <message>
        <source>System &amp;Info</source>
        <translation>&amp;Información del Sistema</translation>
    </message>
    <message>
        <source>F2</source>
        <translation>F2</translation>
    </message>
    <message>
        <source>Autodetection log</source>
        <translation>Log de autodetección</translation>
    </message>
    <message>
        <source>Autodetection &amp;log</source>
        <translation>&amp;Log de autodetección</translation>
    </message>
    <message>
        <source>About</source>
        <translation>Acerca de</translation>
    </message>
    <message>
        <source>&amp;About APPNAME</source>
        <translation>&amp;Acerca de APPNAME</translation>
    </message>
    <message>
        <source>F3</source>
        <translation>F3</translation>
    </message>
    <message>
        <source>Help</source>
        <translation>Ayuda</translation>
    </message>
    <message>
        <source>F1</source>
        <translation>F1</translation>
    </message>
    <message>
        <source>Manage Profiles</source>
        <translation>Gestión de perfiles</translation>
    </message>
    <message>
        <source>Manage &amp;Profiles...</source>
        <translation>Gestión de &amp;Perfiles...</translation>
    </message>
    <message>
        <source>Ctrl+P</source>
        <translation>Ctrl+P</translation>
    </message>
    <message>
        <source>Quit APPNAME</source>
        <translation>Salir de APPNAME</translation>
    </message>
    <message>
        <source>&amp;Quit APPNAME</source>
        <translation>&amp;Salir de APPNAME</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <source>new item</source>
        <translation type="obsolete">nuevo</translation>
    </message>
    <message>
        <source>Select network operator</source>
        <translation>Seleccionar operadora</translation>
    </message>
    <message>
        <source>&amp;Select network operator...</source>
        <translation>&amp;Seleccionar operadora...</translation>
    </message>
    <message>
        <source>Radio Preferences</source>
        <translation>Preferencias de Radio</translation>
    </message>
    <message>
        <source>&amp;Radio Preferences...</source>
        <translation>Preferencias de &amp;Radio...</translation>
    </message>
    <message>
        <source>Disabling PIN protection failed.
Try again, and be sure to use the correct current PIN!</source>
        <translation>Falló la desactivación de la petición de PIN.
Inténtelo de nuevo, y asegurese de usar el código PIN actual corréctamente.</translation>
    </message>
    <message>
        <source>Enabling PIN protection failed.
Try again, and be sure to use the correct current PIN!</source>
        <translation>Falló la activación de la petición de PIN.
Inténtelo de nuevo, y asegurese de utilizar el código PIN actual corréctamente.</translation>
    </message>
    <message>
        <source>PIN protection was successfull.</source>
        <translation type="obsolete">Solicitud de petición de PIN exitosa.</translation>
    </message>
    <message>
        <source>Please enter your current PIN.</source>
        <translation>Por favor, introduzca el PIN actual.</translation>
    </message>
    <message>
        <source>Please enter your NEW pin.</source>
        <translation>Por favor, introduzca su NUEVO PIN.</translation>
    </message>
    <message>
        <source>Please verify your NEW pin!</source>
        <translation type="obsolete">.Por favor, verifique su NUEVO PIN</translation>
    </message>
    <message>
        <source>Changing PIN failed.
New PIN and verify PIN were not same, please try again!</source>
        <translation>Falló el cambio de PIN.
El nuevo PIN y la verificación no coinciden, ¡inténtelo otra vez!</translation>
    </message>
    <message>
        <source>Changing PIN failed.
Try again, and be sure to use the correct current PIN!</source>
        <translation>Falló el cambio de PIN.
Inténtelo otra vez, y asegurese de usar el PIN actual correctamente.</translation>
    </message>
    <message>
        <source>PIN changed successfully.</source>
        <translation>Se cambió el PIN con éxito.</translation>
    </message>
    <message>
        <source>You are not connected to a network yet.
SMS functions are unavailable.</source>
        <translation>Todavía no está conectado a ninguna red.
No se dispone de funciones de SMS.</translation>
    </message>
    <message>
        <source>&lt;&lt;PPP did not provide any stdout information&gt;&gt;</source>
        <translation>&lt;&lt;PPP no proporcionó ninguna información en stdout&gt;&gt;</translation>
    </message>
    <message>
        <source>&lt;&lt;PPP did not provide any stderr information&gt;&gt;</source>
        <translation>&lt;&lt;PPP no proporcionó ninguna información en stderr&gt;&gt;</translation>
    </message>
    <message>
        <source>You are not connected to a network yet.</source>
        <translation type="obsolete">Todavía no está conectado a una red.</translation>
    </message>
    <message>
        <source>Setting up PPP Connection</source>
        <translation type="obsolete">Estableciendo conexión PPP</translation>
    </message>
    <message>
        <source>Failed to start PPP.
Please re-run umtsmon from the commandline with -v5 and check the output.</source>
        <translation>PPP falló.
Por favor, ejecute umtsmon desde la línea de comandos con la opción -v5 y compruebe la salida del programa.</translation>
    </message>
    <message>
        <source>PPP is active, cannot run this in parallel.</source>
        <translation>PPP está activo, no se puede ejecutar en paralelo.</translation>
    </message>
    <message>
        <source>Please enter your NEW pin again.</source>
        <translation>Por favor escriba su NUEVO pin de nuevo.</translation>
    </message>
    <message>
        <source>Your computer just ran into a new month.
Due to limitations of pppd, we cannot account newtraffic to the new month. Please close the connectionand reconnect at your earliest convenience.</source>
        <translation>Su ordenador ha entrado en un nuevo mes.
Debido a las limitaciones de pppd, no podemos contabilizar el tráfico del nuevo mes. Por favor cierre la conexión y reconecte tan pronto como pueda.</translation>
    </message>
    <message>
        <source>You are over your data limit treshold.</source>
        <translation>Se ha excedido su límite de transferencia.</translation>
    </message>
    <message>
        <source>You are over your time limit treshold.</source>
        <translation>Se ha excedido su límite de tiempo.</translation>
    </message>
    <message>
        <source>Se&amp;ttings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PIN Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>  &amp;Enable PIN...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>  &amp;Disable PIN...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>  &amp;Change PIN...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the language</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set Language...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatically connect when network is found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Autoconnect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sorry, the Info dialog cannot be shown
whilst a PPP connection exists.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sorry, the Info dialog cannot be shown
if there is no device detected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disabling PIN protection was successful.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabling PIN protection was successful.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Setting up Internet Connection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Please do not change
the profile whilst connected.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>networkoperator</name>
    <message>
        <source>Network Operator</source>
        <translation type="obsolete">Operadora</translation>
    </message>
    <message>
        <source>Find networks</source>
        <translation type="obsolete">Buscar redes</translation>
    </message>
    <message>
        <source>when disconnected, clicking this button will search for available networks</source>
        <translation type="obsolete">cuando se esté desconectado, pulsando este botón se buscarán redes disponibles</translation>
    </message>
    <message>
        <source>Select</source>
        <translation type="obsolete">Seleccionar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Cancelar</translation>
    </message>
    <message>
        <source>Searching for mobile networks...</source>
        <translation type="obsolete">Buscando redes...</translation>
    </message>
    <message>
        <source>No network operators were found.
You might want to try again.</source>
        <translation type="obsolete">No se encontró ninguna red.
Puede intentarlo de nuevo.</translation>
    </message>
    <message>
        <source>Select your operator from the above list.</source>
        <translation type="obsolete">Seleccione su operadora de la lista de arriba.</translation>
    </message>
    <message>
        <source>Connecting to mobile network...</source>
        <translation type="obsolete">Conectando a red...</translation>
    </message>
</context>
<context>
    <name>profiledialog</name>
    <message>
        <source>Edit Profile</source>
        <translation>Editar Perfil</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Nombre del Perfil:</translation>
    </message>
    <message>
        <source>advanced PPP Options</source>
        <translation>Opciones PPP avanzadas</translation>
    </message>
    <message>
        <source>noauth</source>
        <translation>noauth</translation>
    </message>
    <message>
        <source>Do not require the remote server to authenticate itself. This option is privileged. Usually disabled.</source>
        <translation>No requerir al servidor remoto que se identifique. Está opción requiere privilegios. Normalmente desactivada.</translation>
    </message>
    <message>
        <source>no ip default</source>
        <translation>sin IP por defecto</translation>
    </message>
    <message>
        <source>If enabled, the remote server must supply you an IP Address. Usually disabled.</source>
        <translation>Si activada, el servidor remoto debe proporcionarle una dirección IP. Normalmente desactivado.</translation>
    </message>
    <message>
        <source>replace default route</source>
        <translation>sustituir ruta por defecto</translation>
    </message>
    <message>
        <source>Use the connection for all network traffic when connected. Usually enabled.</source>
        <translation>Usar esta conexión para todo el tráfico de red cuando se conecte. Normalmente desactivado.</translation>
    </message>
    <message>
        <source>set default route</source>
        <translation>establecer ruta por defecto</translation>
    </message>
    <message>
        <source>Add a default route to the remote server if connected. Usually enabled.</source>
        <translation>Añadir una ruta por defecto al servidor remotro cuando se conecte. Normalmente desactivado.</translation>
    </message>
    <message>
        <source>no header compression</source>
        <translation>sin compresión de cabeceras</translation>
    </message>
    <message>
        <source>Header Compression saves a few byte per data packet by skipping information that is already known by the peer. ALWAYS ENABLED.</source>
        <translation>La compresión de cabeceras ahorra unos pocos bytes por cada paquete de datos al ignorar datos que ya son conocidos por el interlocutor remoto. SIEMPRE ACTIVADO.</translation>
    </message>
    <message>
        <source>no CCP</source>
        <translation>sin CCP</translation>
    </message>
    <message>
        <source>Do not negotiate the Compression Control Protocol. Usually disabled - only needed for buggy operators.</source>
        <translation>No negociar el Protocolo de Control de Compresión. Normalmente desactivado - solo se necesita con operadoras que provoquen errores.</translation>
    </message>
    <message>
        <source>use peer DNS</source>
        <translation>usar DNS del interlocutor</translation>
    </message>
    <message>
        <source>Receive DNS-settings from your Provider. Usually enabled.</source>
        <translation>Recibir configuración de DNS de la operadora. Normalmente activado.</translation>
    </message>
    <message>
        <source>login</source>
        <translation>conexión</translation>
    </message>
    <message>
        <source>APN</source>
        <translation>APN</translation>
    </message>
    <message>
        <source>Username</source>
        <translation>Usuario</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Contraseña</translation>
    </message>
    <message>
        <source>monthly data or time limits</source>
        <translation>datos mensuales o limite de tiempo</translation>
    </message>
    <message>
        <source>warn when threshold is reached:</source>
        <translation>avisar cuando se alcance el límite:</translation>
    </message>
    <message>
        <source>min 50%, max 100%</source>
        <translation>min 50%, max 100%</translation>
    </message>
    <message>
        <source>%</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Please enter the megabytes per month that is part of your subscription. This field is for when you are outside your home network - also called roaming.</source>
        <translation>Por favor, introduzca los megabytes por mes que ha contratado. Este campo se usa cuando se conecte a otra operadora que no sea con la que tiene el contrato, también llamado itinerancia(roaming).</translation>
    </message>
    <message>
        <source>roaming:</source>
        <translation>en itinerancia(roaming):</translation>
    </message>
    <message>
        <source>Please enter the megabytes per month that is part of your subscription. This field is for your home network.</source>
        <translation>Por favor, introduzca los megabytes por mes que ha contratado. Este campo se usa cuando se conecte a la operadora con la que tiene el contrato.</translation>
    </message>
    <message>
        <source>MiB/month</source>
        <translation>MB/mes</translation>
    </message>
    <message>
        <source>home network:</source>
        <translation>en operadora contratada:</translation>
    </message>
    <message>
        <source>min/month</source>
        <translation>min/mes</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>There are unsaved changes to your profile!</source>
        <translation>¡Hay cambios al perfil sin guardar!</translation>
    </message>
    <message>
        <source>Your APN contains illegal characters.
You cannot save your profile until you fix this.</source>
        <translation>Su APN contiene caracteres ilegales.
No puede guardar su perfil hasta repararlo.</translation>
    </message>
    <message>
        <source>if your operator requires a username, fill it in here.&lt;br&gt;If not, you probably want to put something dummy in here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>if your operator requires a password, fill it in here.&lt;br&gt;If not, you probably want to put something dummy in here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the changes to the profile.&lt;br&gt;Only enabled if there are changes &lt;b&gt;and&lt;/b&gt; you have entered a valid APN.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>If your subscription is time based,&lt;br&gt;enter the number of minutes whilst roaming per month here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>If your subscription is time based,&lt;br&gt;enter the number of minutes for your home network per month here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Time:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Traffic:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>empty user/pass</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Are you sure you want to leave username or password empty?&lt;br&gt;If you do not specify a username and/or password, connection with the operator may not work as expected.&lt;br&gt;Most operators expect dummy (but non-empty) values here.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>smsdialog</name>
    <message>
        <source>Send SMS</source>
        <translation>Enviar SMS</translation>
    </message>
    <message>
        <source>Send SMS to number:</source>
        <translation>Enviar SMS al número:</translation>
    </message>
    <message>
        <source>enter the number to SMS to here</source>
        <translation>introduzca el número para SMS</translation>
    </message>
    <message>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <source>The number of characters in your message</source>
        <translation>Número de caracteres en su mensaje</translation>
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
        <translation>Número de caracteres que quedan en el mensaje</translation>
    </message>
    <message>
        <source>Characters typed / left:</source>
        <translation>Caracteres tecleados / quedan:</translation>
    </message>
    <message>
        <source>Message:</source>
        <translation>Mensaje:</translation>
    </message>
    <message>
        <source>enter your SMS message here, max 162 characters</source>
        <translation>introduzcal su SMS, máx. 162 caracteres</translation>
    </message>
    <message>
        <source>Clea&amp;r</source>
        <translation>&amp;Limpiar</translation>
    </message>
    <message>
        <source>&amp;Send</source>
        <translation>&amp;Enviar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>sending SMS...</source>
        <translation>enviando SMS...</translation>
    </message>
    <message>
        <source>SMS sent (according to the device).</source>
        <translation>enviado SMS (según el dispositivo).</translation>
    </message>
    <message>
        <source>SMS was not sent!</source>
        <translation>¡no se pudo enviar el SMS!</translation>
    </message>
</context>
</TS>
