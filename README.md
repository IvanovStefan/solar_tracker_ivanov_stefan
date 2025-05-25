# Solar Tracker

Acest proiect constă în realizarea unui suport motorizat pentru un panou solar, capabil să se orienteze automat în funcție de direcția luminii solare. Scopul principal este creșterea eficienței captării energiei prin poziționarea optimă a panoului pe parcursul zilei, pe două axe, orizontal și vertical. De asemenea, va fi afișat pe un ecran și curentul captat de panoul fotovoltaic.

Ideea proiectului a pornit de la observația că majoritatea panourilor solare sunt poziționate static, fără să urmărească soarele, ceea ce duce la o potențială pierdere semnificativă a energiei.

Proiectul este util atât din punct de vedere educațional, deoarece implică concepte studiate la facultate, și din punct de vedere practic, putând fi aplicat în sisteme de captare a energiei solare.

Proiectul este compus din mai multe module care colaborează pentru a asigura orientarea automată a unui panou solar în direcția optimă față de sursa de lumină.

Partea hardware include:

Un panou solar de 6V, montat pe un suport mobil;
Două servomotoare SG90, care permit mișcarea suportului pe două axe;
Patru fotorezistori (LDR 5528), care detectează intensitatea luminii din patru direcții diferite;
Un microcontroller Arduino Uno, care preia valorile de la fotorezistori și controlează poziția servomotoarelor în funcție de diferențele de lumină;
Un senzor de curent (ACS712) care măsoară curentul generat de panou;
Un panou LCD cu interfață I2C, care afișează în timp real curentul măsurat.
Modulul software implementat pe Arduino citește valorile de lumină de la fotorezistori și determină direcția către care panoul trebuie să se orienteze pentru a maximiza captarea luminii. De asemenea, este prelucrată curentului, iar rezultatele sunt afișate pe ecran. Astfel, sistemul asigură o urmărire continuă a sursei de lumină și oferă feedback despre eficiența energetică a orientării curente.

Mai multe detalii despre proiect: [OCW - PM Project]([https://ocw.cs.pub.ro/courses/pm/prj2025/ajipa/stefan.ivanov0604])
