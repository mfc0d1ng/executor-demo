#include <iostream>
#include <string.h>


int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        std::puts("Invalid number of arguments ... program terminated.");
        return EXIT_FAILURE; 
    }
    
    if(strcmp(argv[0], "executor"))
    {
        return EXIT_FAILURE;
    }
    
    if( !( !strcmp(argv[1], "c") || !strcmp(argv[1], "cpp") ) )
    {
        std::printf("Invalid option ==> (%s) ... program terminated.\n", argv[1]); 
        return EXIT_FAILURE;
    }

    /* Opening executor's map file */
    FILE *map_file = fopen("C:\\executor\\map.txt", "r");
    if(!map_file)
    {
        FILE *create_map_file = fopen("C:\\executor\\map.txt", "w");
        if(create_map_file)
        {
            std::fprintf(create_map_file, "%s", "executorMap\n{\n#Example\n\"c\" : \"gcc main.c\",\n\"cpp\" : \"g++ main.cpp\"\n}");
            std::puts("map.txt has been created in this directory \"C:\\executor\", "
                      "You may open map.txt to complete the configurations and run the program again.");  
            std::fclose(create_map_file);  
        }
        else
        {
            std::puts("Couldn't create map.txt in this directory \"C:\\executor\" ... program terminated.");  
            return EXIT_FAILURE; 
        }
        return EXIT_SUCCESS; 
    }

    /* Getting size of executor's map file  */ 
    std::fseek(map_file, 0, SEEK_END);
    std::size_t file_size = ftell(map_file);
    std::fseek(map_file, 0, SEEK_SET); 

    char *file_content = (char *) std::malloc(file_size + 1);
    if(!file_content)
    {
        std::puts("Out of memory ... program terminated.");
        std::fclose(configs_file);
        return EXIT_FAILURE;
    }

    /* Reading content of executor's map file */
    std::fread((void *) file_content, sizeof(char), file_size, configs_file);
    file_content[file_size] = '\0';

    
    std::string tokens = {};

    /* Getting only the tokens, it's what matters. */
    char *token = strtok(file_content, " "); 
    while (token)
    {
        tokens.append(token);
        tokens.push_back(' ');
        token = strtok(NULL, " "); 
    }

    /* Clean up */ 
    std::free(file_content);
    tokens.shrink_to_fit();

    std::string cmd = {}; 
    char *content = (char *) tokens.c_str();
    std::size_t content_size = tokens.size(); 
    int read = 0;

   
    /* Going through executor's map file */
    for (std::size_t i = 12; i < content_size; i++)
    {
        if(content[i] == '#')
        {
            read = 1;  
        }
        if(read == 1 && content[i] == '\n')
        {
            read = 0;
            continue;
        }

        int len = 0;

        if(read == 0 && (
                         ( (!strncmp(&content[i], "\"c\"", len = 3) || !strncmp(&content[i], "\" c \"", len = 5)) && !strcmp(argv[1], "c") )      ||
                         ( (!strncmp(&content[i], "\"cpp\"", len = 5) || !strncmp(&content[i], "\" cpp \"", len = 7)) && !strcmp(argv[1], "cpp") )
                        )
          ) 
        {
            i += strlen(argv[1]) + len + 1;   
            /* Fetch user's configurations */
            for (; content[i] != '"'; ++i)
            {
                if(content[i] == '\n')
                {
                    content[i] = ' '; 
                }
                cmd.push_back(content[i]); 
            }            
            cmd.shrink_to_fit(); 
            break;
        }

    }
    
    /* Execute user's configurations */
    std::system(cmd.c_str());

    /* Clean up */
    std::fclose(map_file);

    return EXIT_SUCCESS; 
}



