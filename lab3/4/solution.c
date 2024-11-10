#include "functions.h"
#include "my_string.h"

int main() {
    Post post;
    return_code state;
    int count_mails = 0;
    
    if (post_init(&post) != OK) {
        printf("Memory allocation error\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    printf("Enter the address of the current post office:\n");
    while ((state = get_address(post.address_ptr)) != OK) {
        if (state == MEMORY_ALLOCATION_ERROR) {
            printf("Memory allocation error\n");
            return MEMORY_ALLOCATION_ERROR;
        }
        printf("\nThe data is incorrect!\n");
        printf("Enter the address of the current post office:\n");
    }

    interactive_dialog(&post, &count_mails);

    return 0;
}
