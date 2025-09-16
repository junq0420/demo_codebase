#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>


#define BUFFER_SIZE 1024

errno_t update_log_record_s(const char* filename, const char* search_str, const char* replace_str);

int main(void)
{
	const char* log_file = "C:\\User\\21035\\OneDrive\\桌面\\zhangsan.log";
	const char* search_str = "888";
	const char* replace_str = "666";

	errno_t result = update_log_record_s(log_file, search_str, replace_str);

	if (result != 0) {
		char error_msg[256];
		strerror_s(error_msg, sizeof(error_msg), result);
		fprintf(stderr, "An error occured: %s\n", error_msg);
	} else {
		printf("Record updated successfully.\n");
	}

	_fcloseall();

	return EXIT_SUCCESS;
}

errno_t update_log_record_s(const char* filename, const char* search_str, const char* replace_str) {

	if (filename == NULL || search_str == NULL || replace_str == NULL) {
		return EINVAL; 	// 返回无效参数错误
	}

	FILE* file_ptr = NULL;

	errno_t err = fopen_s(&file_ptr, filename, "r+");

	if (err != 0 || file_ptr == NULL) {
		char error_msg[256];
		strerror_s(error_msg, sizeof(error_msg), errno);

		fprintf(stderr, "Failed to open config file for reading: %s\n", error_msg);
		exit(EXIT_FAILURE);
	}

	char buffer[BUFFER_SIZE];
	long position = 0;
	int found = 0;

	while (fgets(buffer, BUFFER_SIZE, file_ptr) != NULL) {
		if (strstr(buffer, search_str) != NULL) {
			found = 1;
			position = ftell(file_ptr) - (long)strlen(buffer);
			break;// 找到第一个匹配项之后，立刻停止
		}
	}

	if (found) {
		fseek(file_ptr, position, SEEK_SET); 	// 移动回到找到记录的位置

		// 计算替换文本与源文本的长度差异
		size_t replace_len = strlen(replace_str);
		size_t search_len = strlen(search_str);

		if (replace_len > BUFFER_SIZE - 1 || search_len > BUFFER_SIZE - 1) {
			fclose(file_ptr);

			return ERANGE; // 返回错误码，表示字符串长度超出范围
		}

		// 写入新纪录之前，清除所在位置的行数据
		memset(buffer, ' ', strlen(buffer) - 1);// 用空格填充，保持文件大小不变

		buffer[strlen(buffer) - 1] = '\n';// 保留换行符

		fseek(file_ptr, position, SEEK_SET); // 重新回到标记行的位置

		fputs(buffer, file_ptr); // 清除原来行的内容

		fseek(file_ptr, position, SEEK_SET); // 重新回到标记行的开始

		int result = fputs(replace_str, file_ptr); // 写入替换的字符串

		if (result == EOF) {
			fclose(file_ptr);
			return errno;
		}
	} else {
		fclose(file_ptr);
		return ENOENT; // 返回未找到的匹配项
	}

	fclose(file_ptr);

	return 0;
}


