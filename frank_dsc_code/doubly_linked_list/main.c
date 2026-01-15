#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * 条件编译，根据命令行 -D 指定 IMPL 的值来控制编译哪个文件
 * dll: gcc -DIMPL=1 main.c dll.c  -lpthread -o app_dll
 * dll_adt: gcc -DIMPL=2 main.c dll_adt.c -lpthread -o app_dll_adt
 * cdll: gcc -DIMPL=3 main.c doubly_circle_list.c -lpthread -o app_cdll
 */
#define DLL 1
#define DLL_ADT 2
#define CDLL 3

#ifndef IMPL
#define IMPL DLL_ADT
#endif


#if IMPL == DLL
#	include "dll.h"
#elif IMPL == DLL_ADT
#	include "dll_adt.h"
#elif IMPL == CDLL
#	include "doubly_circle_list.h"
#else
#  error "Please define IMPL=DLL_ADT or IMPL=DLL"
#endif


// Helper function to duplicate a string
char* protable_strdup(const char* str) {
	if (!str) return NULL;
	size_t len = strlen(str);
	char* new_str = (char*)malloc(len + 1);
	if (new_str) {
		strcpy(new_str, str);
	}
	return new_str;
}

#if IMPL == DLL
static void testSongList(void)
{
    DoublyLinkedList *songList = createList();
    if (songList == NULL) {
        perror("Failed to create song list");
        return;
    }

    Song newSong;
    Node *targetNode;

    printf("===== 从后追加歌曲数据 AAA ====\n");
    newSong.title = "AAA"; newSong.artist = "A"; newSong.duration = 342;
    appendList(songList, newSong);

    printf("===== 从前追加歌曲数据 BBB ====\n");
    newSong.title = "BBB"; newSong.artist = "B"; newSong.duration = 245;
    prependList(songList, newSong);

    printf("===== 从后追加歌曲数据 CCC ====\n");
    newSong.title = "CCC"; newSong.artist = "C"; newSong.duration = 187;
    appendList(songList, newSong);

    printf("===== 正序打印歌单 ====\n");
    printListBackward(songList);

    printf("===== 从前追加歌曲数据 DDD ====\n");
    newSong.title = "DDD"; newSong.artist = "D"; newSong.duration = 205;
    prependList(songList, newSong);

    printf("===== 在 AAA 后插入歌曲数据 EEE ====\n");
    newSong.title = "EEE"; newSong.artist = "E"; newSong.duration = 179;
    targetNode = findByTitle(songList, "AAA");
    insertAfter(songList, targetNode, newSong);

    printf("===== 正序打印歌单 ====\n");
    printListBackward(songList);

    printf("===== 删除歌曲 CCC ====\n");
    targetNode = findByTitle(songList, "CCC");
    deleteNode(songList, targetNode);

    printf("===== 正序打印歌单 ====\n");
    printListBackward(songList);

    printf("===== 逆序打印歌单 ====\n");
    printListForward(songList);

}

#elif IMPL == DLL_ADT

// 双向链表测试辅助函数
typedef struct {
	int id;
	char* name;
    int age;
} Employee;

void free_employee(void* data) {

	Employee* emp = (Employee*)data;

	if (emp) {
		printf("Freeing employee ID: %d, Name: %s\n", emp->id, emp->name);
		free(emp->name); // Free the dynamically allocated name
		free(emp);       // Free the employee structure itself
	}
}
int compare_employee_by_id(const void* a, const void* b) {
	const Employee* emp_a = (const Employee*)a;
	const Employee* emp_b = (const Employee*)b;
	return emp_a->id - emp_b->id; // Compare by ID
}
void print_employee_action(void* data, void* context) {
	const Employee* emp = (const Employee*)data;

	// context is not used here, but you can use it if needed
	// TODO 

	if (!emp) {
		printf("Employee data is NULL.\n");
		return; // Check if emp is NULL
	}
	printf("Employee ID: %d, Name: %s, Age: %d\n", emp->id, emp->name, emp->age);
}
void inc_employee_age_action(void *data, void *context) {
    Employee *emp = (Employee *)data;
    if (emp == NULL) {
		printf("Employee data is NULL.\n");
		return; // Check if emp is NULL
    }
    emp->age++;
}


static void testDllAdt(void)
{
	printf("--- Architecture of Doubly Linked List ---\n");

	// 1. Create a new doubly linked list with a memory pool
	printf("Setup 1: Creating a list with a memory pool of 10 nodes...\n");

	DoublyLinkedList* emp_list = createList(10, free_employee);
	if (!emp_list) {
		fprintf(stderr, "Failed to create the doubly linked list.\n");
		return; // Exit if list creation failed
	}

	printf("List created successfully. Initial size: %zu\n", getSizeOfList(emp_list));

	// 2. Append some employees to the list

	printf("Setup 2: Appending employees to the list...\n");

	for (int i = 0; i < 5; i++) {
		Employee* emp = (Employee*)malloc(sizeof(Employee));
		if (emp == NULL) {
			fprintf(stderr, "Memory allocation failed for employee.\n");
			continue; // Skip this iteration if memory allocation fails
		}
		emp->id = 101 + i;
        emp->age = 25 + 2 * i;
		char buffer[50];
		sprintf(buffer, "Employee #%d", emp->id);
		emp->name = protable_strdup(buffer);
		if (emp->name == NULL) {
			fprintf(stderr, "Memory allocation failed for employee name.\n");
			free(emp); // Free the employee structure if name allocation fails
			continue;
		}
		appendList(emp_list, emp);
	}

	// 3. Print the list of employees
	printf("Setup 3: Printing the list of employees...\n");
	forEachList(emp_list, print_employee_action, NULL);
	printf("Current list size: %zu\n", getSizeOfList(emp_list));

	// 4. Find an employee by ID

	printf("Setup 4: Finding an employee with ID 103...\n");

	Employee search_key = { 103, NULL, 0 }; // Create a search key with ID 103

	DListNode* found_node = findNodeInList(emp_list, &search_key, compare_employee_by_id);
	if (found_node) {
		Employee* found_emp = (Employee*)getDataOfNode(found_node);
		printf("Found employee with ID %d, Name: %s, age: %d\n", found_emp->id, found_emp->name, found_emp->age);
	} else {
		printf("Employee with ID 103 not found.\n");
	}

	// 5. Delete an employee from the list
	if (found_node) {
		printf("Setup 5: Deleting employee with ID 103...\n");
		deleteNodeFromList(emp_list, found_node);
		printf("Employee with ID 103 deleted. Current list size: %zu\n", getSizeOfList(emp_list));
	}

	printf("\n --- List after deletion---\n");
	forEachList(emp_list, print_employee_action, NULL);

	// 6. insert value -> head node
	printf("Setup 6: Prepending an employee to the list...\n");
	Employee* ceo = (Employee*)malloc(sizeof(Employee));
    // Before using the 'ceo' pointer, ensure it is not NULL
    if (ceo != NULL) {
        ceo->id = 99;
        ceo->age = 35;
        ceo->name = protable_strdup("CEO");
        if (ceo->name == NULL) {
            fprintf(stderr, "Memory allocation failed for CEO name.\n");
            free(ceo); // Free the employee structure if name allocation fails
        } else {
            prependList(emp_list, ceo);
            printf("Prepended employee with ID %d, Name: %s, Age: %d\n", ceo->id, ceo->name, ceo->age);
			printf("\n--- Final list contents ---\n");
			forEachList(emp_list, print_employee_action, NULL);
			printf("Final list size: %zu\n", getSizeOfList(emp_list));
        }
    } else {
        fprintf(stderr, "Memory allocation failed for CEO.\n");
    }

	// 7. increase employee age
	printf("Setup7: Increase employee age...\n");
    forEachList(emp_list, inc_employee_age_action, NULL);
	printf("\n --- List after inc age ---\n");
	forEachList(emp_list, print_employee_action, NULL);


	// x. Clean up the list
	printf("Cleanup: Destroying the doubly linked list...\n");

	destroyList(&emp_list);
	printf("List destroyed. The pointer emp_list is now NULL: %s\n", 
		emp_list == NULL ? "true" : "false");

	printf("All resources freed successfully.\n");
}

#elif IMPL == CDLL

typedef struct {
	int id;
	int resolutionX;
	int resolutionY;
	char *imageName;
} Image;

void freeImage(void *data)
{
	Image *img = (Image *)data;

	if (img) {
		printf("Freeing image ID: #%d, Name: %s, resolution: %dx%d\n", img->id, img->imageName, img->resolutionX, img->resolutionY);
		free(img->imageName); 
		free(img);       
	}
}
int compareImageById(const void *a, const void *b)
{
	const Image *img1 = (Image *)a;
	const Image *img2 = (Image *)b;
	return img1->id - img2->id;
}
void printImageAction(void *data, void *context)
{
	Image *img = (Image *)data;

	if (!img) {
		printf("Image data is NULL");
		return;
	}
	printf("Image ID: #%d, Name: %s, resolution: %dx%d\n", img->id, img->imageName, img->resolutionX, img->resolutionY);
}

static void testCircularDoublyLinkedListFunc()
{
	printf("--- Architecture of Circular Doubly Linked List ---\n");

	// 1. Create a new circular doubly linked list with a memory pool
	printf("\nSetup 1: Creating a list with a memory pool of 10 nodes...\n");

	CircularDoublyLinkedList *imgList = createCircularList(10, freeImage);
	if (!imgList) {
		fprintf(stderr, "Failed to create the circular doubly linked list.\n");
		return; // Exit if list creation failed
	}

	printf("List created successfully. Initial size: %zu\n", getSizeOfList(imgList));

	// 2. Append some images to the list

	printf("\nSetup 2: Appending images to the list...\n");

	for (int i = 0; i < 5; i++) {
		Image* img = (Image*)malloc(sizeof(Image));
		if (img == NULL) {
			fprintf(stderr, "Memory allocation failed for image.\n");
			continue; // Skip this iteration if memory allocation fails
		}
		img->id = 1 + i;
		img->resolutionX = 320 + 320 * i;
		img->resolutionY = 240 + 240 * i;
		char buffer[50];
		sprintf(buffer, "Image-%03d", img->id);
		img->imageName = protable_strdup(buffer);
		if (img->imageName == NULL) {
			fprintf(stderr, "Memory allocation failed for image name.\n");
			free(img); // Free the employee structure if name allocation fails
			continue;
		}
		appendCircularList(imgList, img);
	}

	// 3. Print the list of employees
	printf("\nSetup 3: Printing the list of employees...\n");
	forEachList(imgList, printImageAction, NULL);
	printf("Current list size: %zu\n", getSizeOfList(imgList));

	// 4. Find an employee by ID

	printf("\nSetup 4: Finding an Image with ID #3...\n");

	Image search_key = { .id = 3, .imageName = NULL, .resolutionX = 0, .resolutionY = 0 }; // Create a search key with ID 103

	CDListNode* foundNode = findNodeInCircularList(imgList, &search_key, compareImageById);
	if (foundNode) {
		Image* foundImg = (Image *)getDataOfNode(foundNode);
		printf("Found Image with ID #%d, Name: %s, resolution: %dx%d\n", foundImg->id, foundImg->imageName, foundImg->resolutionX, foundImg->resolutionY);
	} else {
		printf("Image with ID 3 not found.\n");
	}

	// 5. Delete an employee from the list
	if (foundNode) {
		printf("\nSetup 5: Deleting image with ID #3...\n");
		deleteNodeFromCircularList(imgList, foundNode);
		printf("Image with ID #3 deleted. Current list size: %zu\n", getSizeOfList(imgList));
	}

	printf("\n --- List after deletion---\n");
	forEachList(imgList, printImageAction, NULL);

	// 6. insert value -> head node
	printf("\nSetup 6: Prepending an image to the list...\n");

	Image* favoriteImg = (Image*)malloc(sizeof(Image));
    if (favoriteImg != NULL) {
        favoriteImg->id = 69;
        favoriteImg->resolutionX = 1920;
        favoriteImg->resolutionY = 1080;
        favoriteImg->imageName = protable_strdup("My Love");
        if (favoriteImg->imageName == NULL) {
            fprintf(stderr, "Memory allocation failed for favoriteImg name.\n");
            free(favoriteImg);
        } else {
            prependCircularList(imgList, favoriteImg);
            printf("Prepended image with ID #%d, Name: %s, resolution: %dx%d\n", favoriteImg->id, favoriteImg->imageName, favoriteImg->resolutionX, favoriteImg->resolutionY);
			printf("\n--- Final list contents ---\n");
			forEachList(imgList, printImageAction, NULL);
			printf("Final list size: %zu\n", getSizeOfList(imgList));
        }
    } else {
        fprintf(stderr, "Memory allocation failed for favoriteImg.\n");
    }

	// 7. Rotate forward list
	printf("\nSetup 7: Rotating forward the list...\n");
	rotateForwardList(imgList);
	printf("\n --- List after rotating forward---\n");
	forEachList(imgList, printImageAction, NULL);

	// 8. insert a node after the 'My Love' image with id #69
	printf("\nSetup 9: Inserting a node after the 'My Love' image...\n");
	Image *imgToInsert = malloc(sizeof(Image));
	if (imgToInsert == NULL) {
		fprintf(stderr, "Memory allocation failed for favoriteImg.\n");
	} else {
		imgToInsert->id = 6;
		imgToInsert->resolutionX = 1920;
		imgToInsert->resolutionY = 1080;
		imgToInsert->imageName = protable_strdup("Cute Dog");
		if (imgToInsert->imageName == NULL) {
            fprintf(stderr, "Memory allocation failed for imgToInsert name.\n");
            free(imgToInsert);
		} else {
			search_key.id = 69;
			CDListNode *targetNode = findNodeInCircularList(imgList, &search_key, compareImageById);
			insertAfterTargetNode(imgList, targetNode, imgToInsert);
            printf("Insert image with ID #%d, Name: %s, resolution: %dx%d\n", imgToInsert->id, imgToInsert->imageName, imgToInsert->resolutionX, imgToInsert->resolutionY);
			printf("\n--- Final list contents ---\n");
			forEachList(imgList, printImageAction, NULL);
			printf("Final list size: %zu\n", getSizeOfList(imgList));
		}
	}

	// 9. Rotate backward list
	printf("\nSetup 9: Rotating backward the list...\n");
	rotateBackwardList(imgList);
	printf("\n --- List after rotating backward---\n");
	forEachList(imgList, printImageAction, NULL);

	// x. Clean up the list
	printf("\nCleanup: Destroying the circular doubly linked list...\n");

	destroyCicularList(&imgList);
	printf("List destroyed. The pointer imgList is now NULL: %s\n", imgList == NULL ? "true" : "false");

	printf("All resources freed successfully.\n");
}

#endif


int main(void)
{
#if IMPL == DLL
    testSongList();
#elif IMPL == DLL_ADT
    testDllAdt();
#elif IMPL == CDLL
	testCircularDoublyLinkedListFunc();
#endif

    return 0;
}
