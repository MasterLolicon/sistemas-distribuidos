#include <allegro.h>
#include <stdio.h>
#include <time.h>

int main() {

	SAMPLE *audio;
	int pos, vox;

	// Inicializa la librería de Allegro "liballeg.so.4.4.2" :
	if (install_allegro(SYSTEM_AUTODETECT, NULL, NULL)!= 0) {
		allegro_exit();
		printf("Error: no se puede inicializar la librería ''allegro' !");
		exit(-1);
	}

	// Instala el módulo de sonido:
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL)!= 0) {
		allegro_exit();
		printf("Error: imposible la instalación del módulo de sonido !");
		exit(-1);
	}

	// Sube el archivo wav:
	audio = load_wav("beep.wav");
	if (!audio) {
		allegro_exit();
		printf("Error: imposible subir el archivo !");
		exit(-1);
	}

	// Extrae información general del archivo wav:
	printf("Resolución de muestreo: %dbit\n", audio->bits);
	printf("Canales de salida: %d\n", audio->stereo*-1+1);
	printf("Frecuencia de muestreo: Hz %d\n", audio->freq);
	// Ejecuta el file wav:

	struct timeval tv;
	time_t timeA;
	time_t timeB;

	gettimeofday(&tv, NULL);
	timeB = tv.tv_sec;

	while(1){
		gettimeofday(&tv, NULL);
		timeA = tv.tv_sec;
		if(timeA != timeB){
			timeB = tv.tv_sec;

			vox = play_sample(audio, 255, 128, 1000, 0);
			do {
				pos = voice_get_position(vox);
				printf("Ejecución de la muestra de audio n. %d\r", pos);
			} while (pos!=-1);
		}
	}

	// Terminar: 
	destroy_sample(audio);
	remove_sound();

	return 0;
}