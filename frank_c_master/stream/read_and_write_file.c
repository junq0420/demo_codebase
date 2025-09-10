#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#define BUFFER_SIZE  1024

typedef struct {
	float volume;
	int resolution_x;
	int resolution_y;
	int difficulty;
} GameSetting;

void read_file(const char *filename);
void write_file(const char *filename, const char *msg);
void clear_file(const char *filename);

int update_file(const char *filename, const char *search_str, const char *replaced_str);
void config_file(const char *filename, GameSetting *gs);
void copy_file(const char *src, const char *dest);

int update_file_fixed(const char *filename, const char *search_str, const char *replaced_str);


int main(void)
{
	const char *filename = "./test.txt";
	// GameSetting gs = { 0.75, 1920, 1080, 2 };

	// config_file(filename, &gs);

	// const char *src = "./test.txt";
	// const char *dest = "./cp_test.txt";
	//
	// copy_file(src, dest);

	const char *old = "Engine cooldown";
	const char *new = "2025-09-10 14:33:10 [INFO] This is a replaced string for test the update_file_fixed function";

	if (update_file_fixed(filename, old, new) == 1) {
		puts("Found the strintg in the file and replaced it successfully!");
	} else {
		puts("Not found string in the file");
	}
	

	return 0;
}

void read_file(const char *filename) {
	FILE *fp = NULL;
	char buffer[256];

	fp = fopen(filename, "r");
	if (fp == NULL) {
		// use strerror function to get errno(global variable in <errno.h>) then print relative error msg to console
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		// automatically call fclose to close all opened stream and fflush to flush buffer
		exit(EXIT_FAILURE);
	}

	printf("Prepare to read %s file...\n", filename);
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("> %s", buffer);
		printf("current fp pos: %ld\n", ftell(fp));
	}

	if (ferror(fp)) {
		fprintf(stderr, "Error reading file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} else if (feof(fp)) {
		printf("End of file is reached successfully!\n");
	}
		
	fclose(fp);
}

void write_file(const char *filename, const char *msg) {
	FILE *fp = NULL;

	fp = fopen(filename, "a");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	fprintf(fp, "%s\n", msg);
	if (ferror(fp)) {
		fprintf(stderr, "Error writing file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	fclose(fp);
}

void clear_file(const char *filename) {
	FILE *fp = NULL;

	fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	fclose(fp);
}

int update_file(const char *filename, const char *search_str, const char *replaced_str) {
	if (filename == NULL || search_str == NULL || replaced_str == NULL) {
		perror("filename, search_str or replaced_str is null");
		return EINVAL;
	}

	FILE *fp = NULL;
	char buffer[1024];
	long pos = 0; // record the position for fp
	int found = 0;// is found about searched string

	fp = fopen(filename, "r+");// r+ mode: to open a file for reading and writing
	if (fp == NULL) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (ferror(fp)) {
			fclose(fp);
			perror("Error reading file\n");
			return EXIT_FAILURE;
		}
		if (strstr(buffer, search_str) != NULL) {// found the search_str in file
			pos = ftell(fp);
			found = 1;
			break;
		}
	}

	if (found) {
		printf("current buffer is %s, and its len is %zd", buffer, strlen(buffer));
		pos = pos - strlen(buffer);
		size_t search_str_len = strlen(search_str);
		size_t replaced_str_len = strlen(replaced_str);

		if (search_str_len > BUFFER_SIZE || replaced_str_len > BUFFER_SIZE) {
			fprintf(stderr, "search_str_len(%zd) or replaced_str_len(%zd) great than BUFFER_SIZE(%d)\n", 
		   			search_str_len, replaced_str_len, BUFFER_SIZE);
			fclose(fp);

			return ERANGE;
		}

		fseek(fp, pos, SEEK_SET);
		memset(buffer, ' ', strlen(buffer));
		fputs(buffer, fp);

		fseek(fp, pos, SEEK_SET);
		fputs(replaced_str, fp);
		fputc('\n', fp);

		puts("Replaced string successfully!");
	} else {
		puts("No found searched string in the file!");
	}

	return 0;
}


void config_file(const char *filename, GameSetting *gs) {
	FILE *fp = NULL;

	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	size_t res_w = fwrite(gs, sizeof(GameSetting), 1, fp);
	if (res_w == 0) {
		fprintf(stderr, "Error writing file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		printf("Wrote game setting successfully!\n");
	}
	fclose(fp);

	GameSetting game_setting;
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	size_t res_r = fread(&game_setting, sizeof(GameSetting), 1, fp);
	if (res_r == 0) {
		fprintf(stderr, "Error reading file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		printf("Readed game setting successfully!\n");
		printf("The game setting\nvolume: %.f%%\nresolution: %dx%d\ndifficulty: %d\n", game_setting.volume * 100, 
				game_setting.resolution_x, game_setting.resolution_y, game_setting.difficulty);
	}
	fclose(fp);
}

void copy_file(const char *src, const char *dest) {
	FILE *src_ptr = NULL;
	FILE *dest_ptr = NULL;

	char buffer[BUFFER_SIZE];

	if (src == NULL || dest == NULL) {
		perror("src or dest filename is NULL\n");
		exit(EINVAL);
	}

	src_ptr = fopen(src, "rb");
	if (src_ptr == NULL) {
		fprintf(stderr, "Error opening src file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	dest_ptr = fopen(dest, "wb");
	if (dest_ptr == NULL) {
		fprintf(stderr, "Error opening dest file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	size_t res = 0;
	while ((res = fread(buffer, sizeof(char), BUFFER_SIZE, src_ptr)) != 0) {
		fwrite(buffer, sizeof(char), res, dest_ptr);
		if (ferror(src_ptr)) {
			perror("Error reading src file");
			exit(EXIT_FAILURE);
		}
		if (ferror(dest_ptr)) {
			perror("Error writing dest file");
			exit(EXIT_FAILURE);
		}
	}

	if (res == 0 && feof(src_ptr)) {
		puts("Copied file successfully!");
	}

	fclose(src_ptr);
	fclose(dest_ptr);
}

int update_file_fixed(const char *filename, const char *search_str, const char *replaced_str) {
	if (filename == NULL || search_str == NULL || replaced_str == NULL) {
		perror("filename, search_str or replaced_str is null");
		exit(EINVAL);
	}

	FILE *fp = NULL;
	int res = 0; // record the result for reading and writing
	int found = 0;// is found about searched string
	
	fp = fopen(filename, "r+");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	long file_size = 0;
	fseek(fp, 0, SEEK_END); // set fp to the end of file
	file_size = ftell(fp);
	rewind(fp);

	// allocate temp buffer to save the content of file
	char *temp = (char *)malloc(file_size + 1);
	if (temp == NULL) {
		perror("Failed to malloc for temp");
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	res = fread(temp, sizeof(char), file_size, fp); // read file to temp buffer
	if (res != 0) { printf("read %d bytes from %s\n", res, filename); }
	temp[file_size] = '\0';
	rewind(fp); 

	long pos = 0;
	long search_line_size = 0;
	char buffer[BUFFER_SIZE];
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (ferror(fp)) {
			free(temp);
			fclose(fp);
			perror("Error reading file\n");
			exit(EXIT_FAILURE);
		}
		// find the beginning of the line where search_str is located
		if (strstr(buffer, search_str) != NULL) {
			search_line_size = strlen(buffer);
			pos = ftell(fp);
			pos -= search_line_size;
			found = 1;
			break;
		}

		memset(buffer, 0, BUFFER_SIZE);
	}
	rewind(fp);// reset fp to start point for subsequent rewriting of the file


	if (found) {
		size_t search_str_len = strlen(search_str);
		size_t replaced_str_len = strlen(replaced_str);

		size_t prev_size = (size_t)pos;
		size_t remain_size = (size_t)(file_size - prev_size - search_line_size);

		res = fwrite(temp, prev_size, 1, fp);// write the content before the search_str to file
		if (res != 0) { printf("write %d bytes to %s\n", res, filename); }
		if (ferror(fp)) {
			free(temp);
			fclose(fp);
			perror("Error fwrite file");
			exit(EXIT_FAILURE);
		}

		fprintf(fp, "%s", replaced_str);// write the replaced_str to file
		fputc('\n', fp);

		res = fwrite(temp + prev_size + search_line_size, remain_size, 1, fp);
		if (res != 0) { printf("write %d bytes to %s\n", res, filename); }
		if (ferror(fp)) {
			free(temp);
			fclose(fp);
			perror("Error fwrite file");
			exit(EXIT_FAILURE);
		}
	}

	free(temp);
	fclose(fp);

	return found;
}
