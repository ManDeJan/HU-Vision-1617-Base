# Implementatieplan Edge Detection

Jan Halsema en David de Jong, 23-2-2017

## Doel

Redenen:
- Het leren implementeren van een edge dection algoritme.
- Het leren omgaan met kernels (in relatie tot vision).
- Een voldoende halen voor dit vak.

<!-- We willen graag de gezichtuitlijning in pasfotos gaan herkennen. We willen graag een plaatje waar we alle edges duidelijk zien zodat een computer makkelijk features van een gezicht kan gaan detecteren. -->
Voor gezichtsherkenning in pasfotos is het nodig om eerst de edges te vinden in een foto. Zodat het systeem daarna feature herkenning kan gaan doen.

## Methoden

Om van een zwartwitfoto naar een foto te gaan waar de edges duidelijk zichtbaar op zijn is het nodig om een edge detection toe te passen. Hieronder staan meerdere methoden waarmee dit bereikt wordt.

Verschillende edge detection methoden:

- [Canny edge detection](https://en.wikipedia.org/wiki/Canny_edge_detector)
- [Laplacian of Gaussian](https://en.wikipedia.org/wiki/Blob_detection#The_Laplacian_of_Gaussian)
- [Prewitt](https://en.wikipedia.org/wiki/Prewitt_operator)

Voordelen en nadelen:




                      |                                                                                     voordelen |                                                                                                                                                                                                            nadelen
---------------------:|----------------------------------------------------------------------------------------------:|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:
 Canny edge detection |       Using probability for finding error rate, Localization and response. Improving signal to noise ratio, Better detection specially in noise conditions                                                                                         |
Laplacian of Gaussian | vinden van de juiste plaatsen van de edges en het testen van een groter gebied rond een pixel | Foutgevoellig in de hoeken bij rondingen en op plaatsen waar het niveau van de grijsintensiteid varrieerd. Heeft moeite met het vinden van edgjes in specefieke richting door het gebruik van de Laplacian filter.
              Prewitt |                           Simpelheid van gebruik in het vinden van edges en hun orientatiefo |                                                                                                                                                                                 Gevoelig voor ruis en onnauwkeurig

[[1]](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.301.927&rep=rep1&type=pdf)
source

## Keuze

Wij hebben ervoor gekozen Canny edge detection toe te gaan passen, na verschillende papers gelezen te hebben
zijn we tot de conclusie gekomen dat voor gezichtsherkenning (en de meeste andere situaties) canny edge detection het beste resultaat oplevert.
