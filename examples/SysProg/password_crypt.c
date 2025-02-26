#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h> // To use it, add "-crypt" lib to gcc args

int encrypt_password(const char *password, char **encrypted_password)
{

    char *salt = crypt_gensalt_ra(NULL, 0, NULL, 0);
    if (!salt)
    {
        return -1;
    }

    void *enc_ctx = NULL;
    int enc_cxt_sz = 0;
    char *tmp_encrypted_password = crypt_ra(password, salt, &enc_ctx, &enc_cxt_sz);

    if (tmp_encrypted_password == NULL)
    {
        free(salt);
        return -1;
    }

    *encrypted_password = (char *)calloc((strlen(tmp_encrypted_password) + 1), sizeof(char));

    strcpy(*encrypted_password, tmp_encrypted_password);

    free(enc_ctx);
    free(salt);
    return 0;
}

int compare_passwords(const char *password, const char *hashed_password, int *compare_res)
{

    int cmp_res = 0;

    void *enc_ctx = NULL;
    int enc_cxt_sz = 0;

    char *hashed_entered_password = crypt_ra(password, hashed_password, &enc_ctx, &enc_cxt_sz);
    if (!hashed_entered_password)
    {
        return -1;
    }

    *compare_res = strcmp(hashed_password, hashed_entered_password);

    free(enc_ctx);
    return 0;
}

int main()
{
    char password[256];
    char *hased_pass = NULL;

    printf("Введите пароль: ");
    if (fgets(password, sizeof(password), stdin) == NULL)
    {
        fprintf(stderr, "Ошибка ввода\n");
        return 1;
    }
    password[strcspn(password, "\n")] = '\0';

    if (encrypt_password(password, &hased_pass))
    {
        perror("crypt");
        return 1;
    }

    printf("Хешированный пароль: %s\n", hased_pass);

    char entered_password[256];
    printf("Введите пароль для проверки: ");
    if (fgets(entered_password, sizeof(entered_password), stdin) == NULL)
    {
        fprintf(stderr, "Ошибка ввода\n");
        return 1;
    }

    entered_password[strcspn(entered_password, "\n")] = '\0';

    int compare_res = 0;

    if (compare_passwords(entered_password, hased_pass, &compare_res))
    {
        perror("crypt");
        return 1;
    }

    if (compare_res == 0)
    {
        printf("Пароль верный!\n");
    }
    else
    {
        printf("Неверный пароль!\n");
    }

    free(hased_pass);

    return 0;
}
