#include <iostream>
#include <string.h>


int main(int argc, char *argv[])
{
    if(argc > 3)
    {
        std::puts("Invalid number of arguments ... program terminated.");
        return EXIT_FAILURE; 
    }
    
    if(strcmp(argv[0], "executor"))
    {
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
    
    /* If this condition is fulfilled, Open executor's map file in the specified code editor by user */
    if(argc == 3 && !strcmp(argv[2], "map"))
    {
        std::string open_map_file = {};
        open_map_file.append(argv[1]);
        open_map_file.append(" ");
        open_map_file.append("C:\\executor\\map.txt");
        open_map_file.shrink_to_fit();
        std::system(open_map_file.c_str());
        std::fclose(map_file);
        return EXIT_SUCCESS;
    }
    else if(argc == 3 && strcmp(argv[2], "map"))
    {
        std::printf("Invalid option ==> (%s) ... program terminated.\n", argv[2]); 
        std::fclose(map_file);
        return EXIT_FAILURE;
    }

    if(argc == 2 && !( !strcmp(argv[1], "c") || !strcmp(argv[1], "cpp") ) )
    {
        std::printf("Invalid option ==> (%s) ... program terminated.\n", argv[1]); 
        std::fclose(map_file);
        return EXIT_FAILURE;
    }

    /* Getting size of executor's map file  */ 
    std::fseek(map_file, 0, SEEK_END);
    std::size_t file_size = ftell(map_file);
    std::fseek(map_file, 0, SEEK_SET); 

    char *file_content = (char *) std::malloc(file_size + 1);
    if(!file_content)
    {
        std::puts("Out of memory ... program terminated.");
        std::fclose(map_file);
        return EXIT_FAILURE;
    }

    /* Reading content of executor's map file */
    std::fread((void *) file_content, sizeof(char), file_size, map_file);
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
    tokens.shrink_to_fit();

    /* Clean up */ 
    std::free(file_content);

    std::string cmd = {}; 
    char *content = (char *) tokens.c_str();
    std::size_t content_size = tokens.size(); 
    int read = 0;
    int len = 0;
    
   
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

        if(read == 0 && (
                         ( (!strncmp(&content[i], "\"c\"", len = 3) || !strncmp(&content[i], "\" c \"", len = 5)) && !strcmp(argv[1], "c") )      ||
                         ( (!strncmp(&content[i], "\"cpp\"", len = 5) || !strncmp(&content[i], "\" cpp \"", len = 7)) && !strcmp(argv[1], "cpp") )
                        )
          ) 
        {
            
            if(!strcmp(argv[1], "c"))
            {
                i += strlen(argv[1]) + (len + 3);   
            }
            else if(!strcmp(argv[1], "cpp"))
            {
                i += strlen(argv[1]) + (len + 1); 
            }

            /* Fetch user's configuration */
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
    
    /* Execute user's configuration */
    std::system(cmd.c_str());

    /* Clean up */
    std::fclose(map_file);

    return EXIT_SUCCESS; 
}
