#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MaxSizeData 100
#define MaxSizeStr 1000
#define YES 1
#define NO 0

typedef char * data;
typedef struct node {
    data value;
    struct node * True;
    struct node * False;
} node;

void FindDifference(data person_1, data person_2, node * root);
node * CreateNode(data value);
int IsQuestion(const node * cur);
int AskUser();
void Query(node * root);
int IsExit(data word);
void SaveTree(node * root);
node * DownloadTree();
data FileToStr();
void StrToFile(data str);
data PrintToStr(node * root);
node * Read(data str);
int GetChildStr(char *str, char *l_str, char *r_str);
int FileSizeIsAcceptable(FILE * file);
data GiveRootWord(data str);
void DeleteRootWord(data str);
void DeleteFromStr(data str, int n);
void StrCheck(data str);
int CorrectBracketExpression(data str);
void DeleteTree(node * root);
int FileIsOk(FILE * file);

int main()
{
    node *root = DownloadTree();
    assert(root);

    printf("Akinator v 1.0 welcomes you\n");
    printf("Think up a character and enter any word\n");
    printf("For exit enter 'end'\n");

    data expression = (data)calloc(MaxSizeData, sizeof(char));
    scanf("%[^\n]", expression);
    getchar();
    assert(expression);

    while (!IsExit(expression))
    {
        Query(root);

        scanf("%[^\n]", expression);
        getchar();
        assert(expression);
    }

    free(expression);

    SaveTree(root);

    printf("Goodbye\n");

    return 0;
}

void FindDifference(data person_1, data person_2, node * root)
{
    assert(root);

    data difference = (data)calloc(MaxSizeData, sizeof(char));

    printf("The %s differ from the %s in that it is ", person_1, person_2);
    scanf("%[^\n]", difference);
    getchar();
    assert(difference);

    root->True = CreateNode(person_1);
    root->False = CreateNode(person_2);

    root->value = strdup(difference);

    printf("Successfully\n");
    printf("Think up a character and enter any word\n");
    printf("For exit enter 'end'\n");

    free(difference);
}

node * CreateNode(data value)
{
    assert(value);

    node * current_node = (node *)calloc(1, sizeof(node));

    current_node->value = strdup(value);

    current_node->True = current_node->False = NULL;

    return current_node;
}

int IsQuestion(const node * cur)
{
    assert(cur->True && cur->False || !cur->True && !cur->False);

    return (cur->True && cur->False);
}

void Query(node * root)
{
    assert(root);

    node * cur = root;

    if (IsQuestion(cur))
    {
        printf("%s? (yes/no)\n", cur->value);

        cur = (AskUser())? cur->True:cur->False;
        assert(cur);

        Query(cur);
    }
    else
    {
        printf("This is %s, isn't it?(yes/no)\n", cur->value);

        if (AskUser())
        {
            printf("I know everything! Let's play again\n");
            printf("Think up a character and enter any word\n");
            printf("For exit enter 'end'\n");
        }
        else
        {
            printf("Who it was???\n");
            data person = calloc(MaxSizeData, sizeof(char));
            scanf("%[^\n]", person);
            getchar();
            assert(person);

            FindDifference(cur->value, person, root);

            free(person);
        }
    }
}

int AskUser()
{
    data answer = (data)calloc(MaxSizeData, sizeof(char));
    scanf("%[^\n]", answer);
    getchar();
    char yes[] = "yes";
    char no[] = "no";

    while (strcmp(answer, yes) && strcmp(answer, no))
    {
        printf("Wrong command. Try again\n");
        scanf("%[^\n]", answer);
        getchar();
    }

    if (!strcmp(answer, yes))
        return YES;
    if (!strcmp(answer, no))
        return NO;
}

int IsExit(data word)
{
    char exit[] = "end";

    return !strcmp(exit, word);
}

void StrToFile(data str)
{
    FILE *file;
    file = fopen("base.txt", "w");
    assert(file);

    fprintf(file, "%s", str);
    fprintf(file, "%c", '~');

    fclose(file);
}

int FileSizeIsAcceptable(FILE * file)
{
    int i = 0;
    char symbol;

    fscanf(file, "%c", &symbol);

    while (symbol != '~')
    {
        i++;
        fscanf(file, "%c", &symbol);
    }

    fseek(file, 0, SEEK_SET);

    if (i < MaxSizeStr)
        return YES;
    else
        return NO;
}

data FileToStr()
{
    FILE * file;
    file = fopen("base.txt", "r+w");
    assert(file);
    assert(FileIsOk(file));
    assert(FileSizeIsAcceptable(file));

    data str = (data) calloc(MaxSizeStr, sizeof(char));

    fscanf(file, "%[^~]", str);


    fclose(file);

    int n = strlen(str);

    StrCheck(str);

    return str;
}

void DeleteRootWord(data str)
{
    int i = 0, j;

    while ((str[i] != '(') && (str[i] != '\0'))
            i++;

    int n = strlen(str);

    for (j = 0; j < n - i; j++)
        str[j] = str[j + i];

    str[j] = '\0';
}

data GiveRootWord(data str)
{

    int i = 0;

    if (!str[0])
        return NULL;

    data ret = (data) calloc(MaxSizeData, sizeof(char));

    while ((str[i] != '(') && (str[i] != '\0'))
    {
        ret[i] = str[i];
        i++;
    }
    ret[i] = '\0';

    return ret;
}

void DeleteFromStr(data str, int n)
{
    assert(str);

    int i, len;

    len = strlen(str);

    for (i = 0; i < len - n; i++)
        str[i] = str[i + n];
    str[i] = '\0';
}

int GetChildStr(data str, data l_str, data r_str)
{
    assert(str && l_str && r_str);
    assert(str[0]);

    int i, difference_of_bracket_types = 0;

    DeleteRootWord(str);

    if (!strlen(str))
    {
        l_str[0] = '\0';
        r_str[0] = '\0';
        return 0;
    }

    if (str[0] != '(')
        return 0;
    DeleteFromStr(str, 1);
    difference_of_bracket_types++;

    for (i = 0; difference_of_bracket_types; i++)
    {
        if (str[i] == '(')
            difference_of_bracket_types++;
        if (str[i] == ')')
            difference_of_bracket_types--;
        if (difference_of_bracket_types)
            l_str[i] = str[i];
    }
    l_str[i] = '\0';

    DeleteFromStr(str, i);

    i = 0;
    assert((str[i] == '('));
    DeleteFromStr(str, 1);
    difference_of_bracket_types++;

    for (i = 0; difference_of_bracket_types; i++)
    {
        if (str[i] == '(')
            difference_of_bracket_types++;
        if (str[i] == ')')
            difference_of_bracket_types--;
        if (difference_of_bracket_types)
            r_str[i] = str[i];
    }
    r_str[i] = '\0';

    assert(i == strlen(str));

    return 0;
}

node * Read(data str)
{
    assert(str);

    node * ret = (node *) calloc(1, sizeof(node));

    ret->value = GiveRootWord(str);
    DeleteRootWord(str);

    if (!strlen(str))
    {
        ret->True = ret->False = NULL;

        return ret;
    }


    char l_str[MaxSizeStr] = {}, r_str[MaxSizeStr] = {};

    GetChildStr(str, l_str, r_str);

    ret->True = Read(l_str);
    ret->False = Read(r_str);

    return ret;
}

node * DownloadTree()
{
    data str = FileToStr();
    assert(str);

    node * root = Read(str);
    assert(root);

    return root;
}

void SaveTree(node * root)
{
    data str = PrintToStr(root);
    assert(str);

    StrToFile(str);

    DeleteTree(root);

    free(str);
}

data PrintToStr(node * root)
{
    assert(root);

    data str = (data) calloc(MaxSizeStr, sizeof(char));

    sprintf(str, "%s", root->value);

    if (!IsQuestion(root))
        return str;

    str = strcat(str, "(");

    str = strcat(str, PrintToStr(root->True));

    str = strcat(str, ")(");

    str = strcat(str, PrintToStr(root->False));

    str = strcat(str, ")");

    return str;
}

void StrCheck(data str)
{
    int len = strlen(str);
    int all_is_well = YES;

    if (str[0] == '(')
        all_is_well = NO;

    if (!CorrectBracketExpression(str))
        all_is_well = NO;

    assert(all_is_well);
}

int CorrectBracketExpression(data str)
{
    int len = strlen(str);
    int i;
    int difference_of_bracket_types = 0;

    for (i = 0; i < len; i++)
    {
        if (str[i] == '(')
            difference_of_bracket_types++;
        if (str[i] == ')')
            difference_of_bracket_types--;
        if (difference_of_bracket_types < 0)
            return 0;
    }

    return !difference_of_bracket_types;
}

void DeleteTree(node * root)
{
    if (root->False)
        DeleteTree(root->False);
    if (root->True)
        DeleteTree(root->True);
    free(root);
}

int FileIsOk(FILE * file)
{
    fseek(file, 0, SEEK_END);

    int position = ftell(file);
    fseek(file, position - 2, SEEK_SET);

    char symbol;
    int all_is_ok;

    fscanf(file, "%c", &symbol);

    if (symbol == '~')
        all_is_ok = YES;
    else
        all_is_ok = NO;

    fseek(file, 0, SEEK_SET);

    return all_is_ok;
}
