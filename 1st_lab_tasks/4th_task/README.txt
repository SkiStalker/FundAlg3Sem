Прогнал через makefile ошибки, хотел прогнать и success, но поскольку при нем у нас выводятся результат и в консоль и в отдельный файл, не понял как это завезти так, чтобы makefile это проверял.
(пример похожей ситуации, пробовал его, не получилось..)
upd: ../test_templates/success_rfile_ofile: line 8: ../test_templates/template_result_file_outputfile: No such file or directory //категорически не хочет видеть template_result_file_outputfile
Поэтому прикрепляю исходный файл test.txt и файлы test_a_output, test_d_output, test_i_output, test_s_output с результатами вывода программы в зависимости от флага.
