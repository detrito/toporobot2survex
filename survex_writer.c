/* file: survex_writer.c
 * author: detrito
 * date: january 2016
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "survex_writer.h"
#include "functions.h"
#include "container.h"

int write_buffer(char *filename) {
	init_output_file(filename); // clear file or create path

	FILE *file = fopen(filename, "a"); 	// open file in appening mode
	
	if (file != NULL) {
		fputs(buffer, file); // write buffer to file
		return fclose(file);
	}
	else {
		printf("Error: impossible to open the file \"%s\" in appening mode.\n",
			filename);
		exit(1);
	}
}

void survex_write_cave(const char *folder) {
	Serie *serie;
	
	strcpy(foldername, folder);
	if(verbose) {
		printf("Writing cave... \n");
	}
	
	survex_write_main(cave);
	
	for(int i=1; i<=cave_get_series_length(cave); i++) {
		serie = cave_get_serie(cave, i);

		if(serie != NULL) {
			if(serie) {
				if(verbose) {
					printf("Writing serie %d...\n", i);
				}
				survex_write_serie(serie);
			}
		}
	}
}

// write the main file who includes all the series
void survex_write_main() {

	// assemble filename
	strcpy(filename,"\0");
	append_to_str(filename, sizeof(filename), "%s/main.svx", foldername);
	
	// puts
	strcpy(buffer,"\0");
	append_to_str(buffer, sizeof(buffer), "*begin\n");
	append_to_str(buffer, sizeof(buffer), "*title \"%s\"\n", cave->name);

	// blank line
	append_to_str(buffer, sizeof(buffer), "\n");
	
	// entrance's coordinates
	append_to_str(buffer, sizeof(buffer), "*equate entree 1.0\n");
	append_to_str(buffer, sizeof(buffer), "*fix entree %d %d %d\n\n",
		cave->entrance.x, cave->entrance.y, cave->entrance.z);
	
	// include all serie files
	for(int i=1; i<=cave_get_series_length(cave); i++) {
		serie = cave_get_serie(cave, i);
		
		if(serie) {
			append_to_str(buffer, sizeof(buffer), "*include surveys/%d\n",
				serie->id_serie);
		}
	}
	
	// blank line
	append_to_str(buffer, sizeof(buffer), "\n");
	
	// links series together
	for(int i=1; i<=cave_get_series_length(cave); i++) {
		serie = cave_get_serie(cave, i);
		
		if(serie) { 
			// link to first measure
			append_to_str(buffer, sizeof(buffer), "*equate %d.%d %d.%d\n",
				serie->id_serie,
				0,
				serie->link_begin_serie,
				serie->link_begin_measure);
			// link to last measure
			append_to_str(buffer, sizeof(buffer), "*equate %d.%d %d.%d\n",
				serie->id_serie,
				serie_get_measures_length(serie)-1,
				serie->link_end_serie,
				serie->link_end_measure);
		}
		
		
	}
	
	// blank line and end
	append_to_str(buffer, sizeof(buffer), "\n*end\n");
	write_buffer(filename);
}


void survex_write_serie(Serie *serie)
{
	char unit_azimuth[16];
	char unit_dip[16];

	// assemble filename
	
	strcpy(filename,"\0");
	append_to_str(filename, sizeof(filename),
		"%s/surveys/%d.svx", foldername, serie->id_serie);

	// begin serie
	strcpy(buffer,"\0"); 	// empty buffer
	
	// serie begin and newline
	append_to_str(buffer, sizeof(buffer), "*begin %d\n\n", serie->id_serie);

	// serie title	
	append_to_str(buffer, sizeof(buffer), "*title \"%s\"\n", serie->name);
	
	// measure corrections
	survey = serie_get_survey(serie);
	if(survey)
	{
		// azimuth correction
		if(survey->correction_azimuth != 0) {
			append_to_str(buffer, sizeof(buffer), "*CALIBRATE COMPASS %.2f\n",
				survey->correction_azimuth);
		}
	
		// dip correction
		if(survey->correction_dip != 0) {
			append_to_str(buffer, sizeof(buffer), "*CALIBRATE CLINO %.2f\n",
				survey->correction_dip);
		}
	}
		
	// units
	code = serie_get_code(serie);
	if(code) {		
		switch (code->unit_azimuth) {
			case 360:
				strcpy(unit_azimuth,"DEGREES");
				break;
			case 400:
				strcpy(unit_azimuth,"GRADS");
				break;
			default:
				fprintf(stderr,"Error: unexpexted azimuth unit\n");
				exit(1);
		}
		append_to_str(buffer,sizeof(buffer), "*UNITS COMPASS %s\n", unit_azimuth);
		
		switch (code->unit_dip) {
			case 360:
				strcpy(unit_dip,"DEGREES");
				break;
			case 400:
				strcpy(unit_dip,"GRADS");
				break;
			case 370:
				strcpy(unit_dip,"PERCENT");
				break;			
			default:
				fprintf(stderr,"Error: unexpexted dip unit\n");
				exit(1);
		}
		append_to_str(buffer, sizeof(buffer), "*UNITS CLINO %s\n", unit_dip);
	}
	
	
	// append measures
	append_to_str(buffer, sizeof(buffer),
		"\n*data normal from to tape compass clino\n");
	
	for(int i=0; i<serie_get_measures_length(serie); i++) {
		measure = serie_get_measure(serie, i);
		append_to_str(buffer, sizeof(buffer), "%d\t%d\t%.2f\t%.2f\t%.2f\n",
			i,
			i+1,
			measure->length,
			measure->azimuth,
			measure->dip
		);
	}
	
	// append passage
	append_to_str(buffer, sizeof(buffer),
		"\n*data passage station left right up down ignoreall\n");
	
	for(int i=0; i<serie_get_measures_length(serie); i++) {
		measure = serie_get_measure(serie, i);
		append_to_str(buffer, sizeof(buffer), "%d\t%d\t%.2f\t%.2f\t%.2f\t%.2f\n",
			i,
			i+1,
			measure->left,
			measure->right,
			measure->up,
			measure->down
		);
	}
	
	// blank line and end serie
	append_to_str(buffer, sizeof(buffer), "\n*end %d\n",serie->id_serie);
	
	// write to file
	write_buffer(filename);
}
