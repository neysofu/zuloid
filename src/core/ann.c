#include <stdio.h>
//#include <tensorflow/tensorflow/c/c_api.h>
/*
struct Model {
	struct TF_Session tf_session;

};

void
model_boot(void) {

}

void
model_drop(void) {
	TF_CloseSession(model->tf_session, model->tf->status);
}

int main() {
	TF_Buffer* graph_def = read_file("tensorflow_inception_graph.pb");
	TF_Graph* graph = TF_NewGraph();
	// Import graph_def into graph
	TF_Status* status = TF_NewStatus();
	TF_ImportGraphDefOptions* opts = TF_NewImportGraphDefOptions();
	TF_GraphImportGraphDef(graph, graph_def, opts, status);
	TF_DeleteImportGraphDefOptions(opts);
	if (TF_GetCode(status) != TF_OK) {
	        fprintf(stderr, "ERROR: Unable to import graph %s", TF_Message(status));
	        return 1;
	}
	fprintf(stdout, "Successfully imported graph");
	TF_DeleteStatus(status);
	TF_DeleteBuffer(graph_def);
	// Use the graph
	TF_DeleteGraph(graph);
	return 0;
}

TF_Buffer* read_file(const char* file) {
	FILE *f = fopen(file, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  //same as rewind(f);
	void* data = malloc(fsize);
	fread(data, fsize, 1, f);
	fclose(f);
	TF_Buffer* buf = TF_NewBuffer();
	buf->data = data;
	buf->length = fsize;
	buf->data_deallocator = free_buffer;
	return buf;
}*/
