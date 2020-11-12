XLS_PATH?=
XLS_IR_CONVERTER=$(XLS_PATH)/bazel-bin/xls/dslx/ir_converter_main
XLS_OPT=$(XLS_PATH)/bazel-bin/xls/tools/opt_main
XLS_CODEGEN=$(XLS_PATH)/bazel-bin/xls/tools/codegen_main
MWC:=../utils/multi-word-converter

OUTPUT_MODULES=cubic-bezier.sv

all: $(OUTPUT_MODULES)

%.ir: %.x
	$(eval SNAKE_NAME:=$(shell $(MWC) dash snake $*))
	cp $< $(SNAKE_NAME)_private.x
	$(XLS_IR_CONVERTER) $(SNAKE_NAME)_private.x > $*.ir
	rm $(SNAKE_NAME)_private.x

%.opt.ir: %.ir
	$(XLS_OPT) $< > $@

%.sv: %.opt.ir
	$(XLS_CODEGEN) --use_system_verilog --pipeline_stages=1 --delay_model=unit $< > $@

$(MWC):
	$(MAKE) -C ../utils multi-word-converter

clean:
	rm -rf $(OUTPUT_MODULES) *.ir *_private.x
