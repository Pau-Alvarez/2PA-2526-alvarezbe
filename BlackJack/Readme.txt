Para ejecutar este proyecto, yo he estado utilizando su versión en debug para ello necesitaremos cualquier versión de Visual estudio, el archivo premake5 incluido en la carpeta y finalmente un archivo que hay que añadir que sería el premake5.exe
Una vez tenemos todo lo necesario abrimos la consola de visual studio correspondiente a la versión con la que queremos trabajar y ponemos los siguientes comandos:

-cd [la diercción de nuestro directorio]
-premake5 vs[la versión de nuestro visual studio]

esto generará una carpeta build con la que al abrir con visual studio podremos ejecutar.


----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Para generar el documento Doxygen necesitaremos tener Doxygen intalado y acto seguido realizar los siguientes comando en nuestra consola:

-cd [la diercción de nuestro directorio]
-doxygen Doxyfile

Aparecerá un index.html que contendrá la documentación del proyecto.
