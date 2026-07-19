/******************************************************************************************************************************************
 *                                                                                                                                        *
 * embed_cpp_binary - Tool for converting binary files into embeddable C++ headers.                                                       *
 *                                                                                                                                        *
 * Usage: embed_cpp_binary INPUT OUTPUT NAME                                                                                              *
 *  - INPUT: input file path                                                                                                              *
 *  - OUTPUT: output file path                                                                                                            *
 *  - NAME: C++ byte array variable name                                                                                                  *
 *                                                                                                                                        *
 ******************************************************************************************************************************************/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

/* Signals the program to continue executing. */
#define CONTINUE -1

/* Validates the command-line arguments passed to embed_cpp_binary. */
int validate_command_line_args(int argc, char* argv[])
{
	const char* it;

	if (argc != 4) {
		int exit_code;

		exit_code = !(argc <= 1 || strcmp(argv[1], "help") || strcmp(argv[1], "--help"));
		fprintf(exit_code == 0 ? stdout : stderr,
				"Usage: %s INPUT OUTPUT NAME\n"
				"Convert text file at INPUT into an #include-able C++ header containing a byte array called NAME at OUTPUT.\n",
				argc > 0 ? argv[0] : "embed_cpp_binary");
		return exit_code;
	}
	for (it = argv[3]; *it != '\0'; ++it) {
		if (*it != '_' && (!isalnum(*it) || (it == argv[3] && isdigit(*it)))) {
			fprintf(stderr, "embed_cpp_binary: error: Invalid variable name '%s'", argv[3]);
			return 1;
		}
	}

	return CONTINUE;
}

/* Opens the input and output files. */
int open_files(const char* input_path, FILE** input, const char* output_path, FILE** output)
{
	*input = fopen(input_path, "r");
	if (!input) {
		fprintf(stderr, "embed_cpp_binary: %s: %s", input_path, strerror(errno));
		return 1;
	}
	*output = fopen(output_path, "w");
	if (!output) {
		fprintf(stderr, "embed_cpp_binary: %s: %s", output_path, strerror(errno));
		fclose(*input);
		return 1;
	}

	return CONTINUE;
}

/* Writes to the output file. */
int write_to_output(const char* input_path, FILE* input, const char* output_path, FILE* output, const char* name)
{
	int byte;

	if (fprintf(output, "constexpr std::byte %s[] = {\n", name) < 0) {
		fprintf(stderr, "embed_cpp_binary: %s: Error while writing file", output_path);
		return 1;
	}
	while ((byte = fgetc(input)) != EOF) {
		if (fprintf(output, "\tstd::byte{0x%02x},\n", byte) < 0) {
			fprintf(stderr, "embed_cpp_binary: %s: Error while writing file", output_path);
			return 1;
		}
	}
	if (ferror(input)) {
		fprintf(stderr, "embed_cpp_binary: %s: Error while reading file", input_path);
		return 1;
	}
	if (fputs("};", output) == EOF) {
		fprintf(stderr, "embed_cpp_binary: %s: Error while writing file", output_path);
		return 1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int result;
	FILE* input;
	FILE* output;

	result = validate_command_line_args(argc, argv);
	if (result != CONTINUE) {
		return result;
	}

	result = open_files(argv[1], &input, argv[2], &output);
	if (result != CONTINUE) {
		return result;
	}

	result = write_to_output(argv[1], input, argv[2], output, argv[3]);
	fclose(input);
	fclose(output);
	return result;
}