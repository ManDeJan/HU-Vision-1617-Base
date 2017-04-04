
# Implementatieplan Edge Detection

Jan Halsema en David de Jong, 23-2-2017
## Inleiding

Voor gezichtsherkenning in pasfoto’s is het nodig om eerst de edges (harde lijnen) te vinden in een foto zodat het gezicht
geïdentificeerd kan worden binnen een afbeelding.
Om dit te bereiken wordt er in het algemeen gebruik gemaakt van edge detection, algoritmes die naar de data van een
afbeelding kijken om de harde lijnen zichtbaar te maken voor andere algoritmes.


## Doel

Ons doel is om te kijken of we meer details in het gezicht kunnen vinden door het implementeren van een ander edge detection algoritme.


Onze persoonlijke doelen zijn die wij hiermee willen bereiken zijn:
- Het leren implementeren van een edge dection algoritme.
- Het leren omgaan met kernels (in relatie tot vision).
- Het gebruik van thresholding om het uiteindelijk resultaat te verduidelijken.

met natuurlijk als uiteindelijk doel om een voldoende te halen voor het vak Vision.

<!-- We willen graag de gezicht uitlijning in pasfoto’s gaan herkennen. We willen graag een plaatje waar we alle edges duidelijk zien zodat een computer simpel features van een gezicht kan gaan detecteren. -->

## Methoden
Om dit doel te bereiken hebben wij kort onderzoek gedaan naar verschillende methodes van edge detection.
Hieruit zijn de volgende kandidaten gebleken:

<!-- Verschillende edge detection methoden: -->

- [Canny edge detection](https://en.wikipedia.org/wiki/Canny_edge_detector)
- [Laplacian of Gaussian[1]](https://en.wikipedia.org/wiki/Blob_detection#The_Laplacian_of_Gaussian)
- [Laplacian of Gaussian[2]](http://homepages.inf.ed.ac.uk/rbf/HIPR2/log.htm)
- [Prewitt](https://en.wikipedia.org/wiki/Prewitt_operator)
- [Sobel](https://en.wikipedia.org/wiki/Sobel_operator)

Elke methode heeft natuurlijk zijn voor en nadelen, Sobel en Prewitt zijn beiden simpel te implementeren maar zijn gevoelig voor ruis
en zijn vaak inaccuraat. Een zero crossing methode zoals Laplacian heeft als voordeel dat het in één kernel edges in alle richtingen zoekt. Dit resulteert er alleen in dat deze kernel ook kan reageren op edges die al gevonden zijn wat voor een
vervorming of kleine verplaatsing kan zorgen. De Laplacian of Gaussian telt ook nog een gaussian kernel er bij op waardoor je in een keer smooth en edges detect waardoor het minder last heeft van ruis. De grootte van de kernel lijd er alleen toe dat er fouten worden gecreëerd op hoeken en curves waar de intensiteit van het grijsniveau verschilt. Als laatste hebben we nog Canny edge detection waarin vooral naar de hiervoor genoemde methodes word gekeken en het beste van alles met elkaar gecombineerd wordt om tot een verbeterd resultaat te komen. Dit maakt het nadeel van Canny meteen duidelijk:
Canny is moelijk te implementeren door de complexe berekeningen die het gebruikt en neemt ook meer tijd in beslag door al deze extra stappen.




## Keuze

Na alle voordelen en nadelen op een rij te hebben gezet [tabel 1] is uiteindelijk onze keuze op Canny gevallen. Dit omdat met het implementeren van Canny veel delen uit andere edge detection methoden haalt, waardoor hij minder last heeft van ruis en gedetaileëerdere edges detecteerd en de locatie daarvan nauwkeuriger is.


tabel 1

methode                   | voordelen                                                                                         | nadelen
--------------------------|---------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Canny edge detection      | Uitstekende prestaties in plaatjes met ruis, en beter in het vinden van edges in alle richtingen  | Vrij complexe berekeningen en daardoor een "zwaar" algoritme
Laplacian of Gaussian     | Vinden van de juiste plaatsen van de edges door het testen van een groter gebied rondom een pixel | Fout gevoelig in de hoeken bij rondingen en op plaatsen waar het niveau van de grijs intensiteit varieert. Heeft moeite met het vinden van edges in specifieke richting door het gebruik van de Laplacian filter.
Prewitt / Sobel           | Simpelheid van gebruik in het vinden van edges en hun oriëntatie                                  | Gevoelig voor ruis en onnauwkeurig
Zero crossing (Laplacian) | Detectie van edges in alle directies. Vaste kenmerken in alle richtingen.                         | Reageert op al gevonden edges. Gevoelig voor ruis.

[Bron [1]](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.301.927&rep=rep1&type=pdf)





Wij realiseren ons dat Canny moelijker te implementeren is dan andere methodes. Om te voorkomen dat wij vast komen te zitten bij de implementatie hebben wij al een klein vooronderzoek gedaan naar de Canny methode.
Sources: [[1]](https://github.com/hasanakg/Canny-Edge-Detector/blob/master/Canny%20Edge%20Detect/canny.cpp) [[2]](https://github.com/opencv/opencv/blob/05b15943d6a42c99e5f921b7dbaa8323f3c042c6/modules/imgproc/src/canny.cpp) [[3]](http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html)


## Implementatie

Om Canny te implementeren gaan wij een header file aanmaken met de functies die nodig zijn om Canny te implementeren.
Deze functies zullen uiteindelijk worden aangeroepen via het studentengedeelte van het programma.

De eerste functie die nodig is voor de implementatie van Canny is een functie die een [Gaussian function](https://en.wikipedia.org/wiki/Gaussian_function) kan omzetten in een kernel zoals [tabel 2].

tabel 2

0.003765     | 0.015019     | 0.023792     | 0.015019     | 0.003765
-------------|--------------|--------------|--------------|-------------
**0.015019** | **0.059912** | **0.094907** | **0.059912** | **0.015019**
**0.023792** | **0.094907** | **0.150342** | **0.094907** | **0.023792**
**0.015019** | **0.059912** | **0.094907** | **0.059912** | **0.015019**
**0.003765** | **0.015019** | **0.023792** | **0.015019** | **0.003765**

De tweede stap is een functie die meerdere kernels op een afbeelding kan toepassen en hier het gecombineerde resultaat van terug kan geven.
Op deze manier is het mogelijk om bijvoorbeeld een Sobel operatie die binnen Canny word gebruikt in een keer uit te voeren zonder de afbeelding tussentijds op te slaan waardoor een gedeelte van het resultaat verloren zou kunnen gaan.
De derde stap is het vinden van de gradiënt richting.
We vinden de richting door de inverse tanges van de Y gradient gedeelt door de X gradient te nemen. Dit slaan we op in een apparte vector om deze bij stap 4 te gebruiken voor non maximum surpression.
Nu we de richting van elke edge weten gaan we deze onderscheiden in vier groepen van links naar rechts, boven naar beneden, en de twee diagonalen. Daarna gaan we voor alle pixels af of ze sterkers zijn dan hun 2 buren in hun richting. Als ze dat niet zijn, maken we de edge zwart.
Hierna gaan we de vijfde stap doen, WAAAAAAH-thresholding, deze hysterische threshold maakt eigenlijk gebruik van twee thresholds. Dingen boven de hoogste threshold blijven er zowiezo in, en dingen boven de lage threshold blijven er alleen in als ze verbonden zijn via pixels hoger dan de lage threshold aan een pixel die boven de hogere threshold valt.
Pixels onden de lage threshold worden op 0 gezet.


## Evaluatie

Wij willen gaan testen op het resultaat van onze edge detection en deze vergelijken met de bestaande edge detection. Daarna gaan we kijken welke het beste resultaat heeft met betrekking tot face recognition.
Dit gaan wij testen door te kijken hoe de features worden geplaatst op de gezichten en kijken welke van de implementaties het dichtst bij de waarheid zit.
Wij denken dat onze edge detection beter is omdat wij de locatie van de edges met Canny accurater weer kunnen geven.
