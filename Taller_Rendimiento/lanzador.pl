#!/usr/bin/perl
#**************************************************************
#         		Pontificia Universidad Javeriana
#     Autor: Carolina Ujueta Ricardo
#     Fecha: 13 de noviembre, 2025
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************/

$Path = `pwd`;
chomp($Path);

$Nombre_Ejecutable = "mmClasicaFork";
@Size_Matriz  = ("2","10","20");
@Num_Hilos    = (1,2,4,8,16,20);
$Repeticiones = 4;

foreach $size (@Size_Matriz) {
    foreach $hilo (@Num_Hilos) {

        $file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";

        # Abrir archivo en modo append (agregar al final)
        open(FH, ">>", $file) or die "No se puede abrir $file: $!";

        for ($i = 0; $i < $Repeticiones; $i++) {
            # Ejecutar el programa y capturar la salida
            $salida = `$Path/$Nombre_Ejecutable $size $hilo`;

            # Escribir la salida en el archivo
            print FH $salida;
        }

        # Cerrar el archivo
        close(FH);
    }
}
