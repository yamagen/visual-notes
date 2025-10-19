#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#define image_width 60
#define image_dir "images/"

void write_index(json_t *array, const char *output_filename) {
    FILE *out = fopen(output_filename, "w");
    if (!out) {
        perror("fopen");
        return;
    }

    fprintf(out, "# Visual Intex\n\n");

    size_t index;
    json_t *item;
    json_array_foreach(array, index, item) {
        const char *filename = json_string_value(json_object_get(item, "filename"));
        if (!filename) continue;

        json_t *date_arr = json_object_get(item, "date");
        const char *date_str = "unknown";
        char date_buf[16] = {0};
        if (json_is_array(date_arr) && json_array_size(date_arr) > 0) {
            json_int_t date_val = json_integer_value(json_array_get(date_arr, 0));
            snprintf(date_buf, sizeof(date_buf), "%lld", date_val);
            date_str = date_buf;
        }

        json_t *tags = json_object_get(item, "tags");
        char tags_buf[256] = {0};
        if (json_is_array(tags)) {
            size_t i;
            for (i = 0; i < json_array_size(tags); i++) {
                const char *tag = json_string_value(json_array_get(tags, i));
                if (i > 0) strcat(tags_buf, ", ");
                strcat(tags_buf, tag);
            }
        }

        fprintf(out, "## <img src=\"%s%s.png\" width=\"%d\">\n", image_dir, filename, image_width);
        fprintf(out, "- %s (%s)\n", filename, date_str);
        fprintf(out, "- Tags: %s\n", tags_buf);
        fprintf(out, "- Files: ");
        json_t *exts = json_object_get(item, "extension");
        if (json_is_array(exts)) {
            size_t i;
            for (i = 0; i < json_array_size(exts); i++) {
                const char *ext = json_string_value(json_array_get(exts, i));
                fprintf(out, "[`%s`](%s%s.%s)", ext, image_dir, filename, ext);
                if (i < json_array_size(exts) - 1) fprintf(out, ", ");
            }
        }
        fprintf(out, "\n\n");
    }

    fclose(out);
    printf("Wrote to %s\n", output_filename);
}

int main(int argc, char *argv[]) {
    const char *input_filename = "index.json";
    const char *output_filename = "index.md";

    if (argc > 1) input_filename = argv[1];
    if (argc > 2) output_filename = argv[2];

    json_error_t error;
    json_t *root = json_load_file(input_filename, 0, &error);
    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return 1;
    }

    if (!json_is_array(root)) {
        fprintf(stderr, "JSON root is not an array\n");
        json_decref(root);
        return 1;
    }

    write_index(root, output_filename);
    json_decref(root);
    return 0;
}
