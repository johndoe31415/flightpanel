#!/usr/bin/python3
import collections

class IOMuxPinmap(object):
	_Pin = collections.namedtuple("Pin", [ "lineno", "pintype", "pinno", "name" ])

	def __init__(self, input_filename):
		self._number_mux = None
		self._current = None
		self._inputs = { }
		self._outputs = { }
		self._parse(input_filename)

	def _parseline(self, lineno, line):
		if (line == "") or line.startswith("#"):
			return
		if line.startswith("~"):
			# Number of multiplexers
			assert(self._number_mux is None)
			self._number_mux = int(line[1:])
			return

		if line.startswith("*"):
			self._current = line[1:].strip().lower()
			assert(self._current in [ "in", "out" ])
			return

		assert(self._current is not None)

		(pin_no, name) = line.split(maxsplit = 1)
		pin_no = int(pin_no)
		if self._current == "in":
			pindict = self._inputs
		else:
			pindict = self._outputs

		if pin_no in pindict:
			raise Exception("%s pin %d, defined in line %d already previously defined: %s" % (self._current, pin_no, lineno, str(pindict[pin_no])))
		pin = self._Pin(lineno = lineno, pintype = self._current, pinno = pin_no, name = name)
		pindict[pin_no] = pin

	def _parse(self, filename):
		with open(filename) as f:
			for (lineno, line) in enumerate(f, 1):
				line = line.rstrip("\r\n")
				self._parseline(lineno, line)

	def _print_cols(self, f, *cols):
		print("\t".join(cols), file = f)

	def _write_header(self, f):
		print("#ifndef __IOMUX_PINMAP_H__", file = f)
		print("#define __IOMUX_PINMAP_H__", file = f)
		print(file = f)
		print("struct iomux_pin_t {", file = f)
		print("	int pinno;", file = f)
		print("	const char *name;", file = f)
		print("};", file = f)
		print(file = f)
		io_count = self._number_mux * 8
		self._print_cols(f, "#define IOMUX_MULTIPLEXER_COUNT", "%d" % (self._number_mux))
		self._print_cols(f, "#define IOMUX_IO_COUNT", "(IOMUX_MULTIPLEXER_COUNT * 8)", "// %d" % (io_count))
		print(file = f)

		print("// %d inputs (%d free). ID range %d - %d:" % (len(self._inputs), io_count - len(self._inputs), min(self._inputs), max(self._inputs)), file = f)
		self._print_cols(f, "#define IOMUX_INPUT_COUNT", str(len(self._inputs)))
		for pin in sorted(self._inputs.values()):
			print("#define IOMUX_IN_%s		%d" % (pin.name, pin.pinno), file = f)
		print(file = f)
		print(file = f)

		print("// %d outputs (%d free). ID range %d - %d:" % (len(self._outputs), io_count - len(self._outputs), min(self._outputs), max(self._inputs)), file = f)
		self._print_cols(f, "#define IOMUX_OUTPUT_COUNT", str(len(self._outputs)))
		for pin in sorted(self._outputs.values()):
			print("#define IOMUX_OUT_%s		%d" % (pin.name, pin.pinno), file = f)

		print(file = f)
		print("const struct iomux_pin_t *iomux_get_input_description(int pin_no);", file = f)
		print("const struct iomux_pin_t *iomux_get_output_description(int pin_no);", file = f)
		print(file = f)
		print("#endif", file = f)

	def _write_array(self, f, pins, name, count):
		print("static const struct iomux_pin_t %s[%s] = {" % (name, count), file = f)
		for pin in pins:
			print("	{", file = f)
			print("		.pinno = %d," % (pin.pinno), file = f)
			print("		.name = \"%s\"," % (pin.name), file = f)
			print("	},", file = f)
		print("};", file = f)
		print(file = f)

	def _write_accessor(self, f, pintype, count):
		print("const struct iomux_pin_t *iomux_get_%s_description(int pin_no) {" % (pintype), file = f)
		print("	for (int i = 0; i < %s; i++) {" % (count), file = f)
		print("		if (%s_pins[i].pinno == pin_no) {" % (pintype), file = f)
		print("			return &%s_pins[i];" % (pintype), file = f)
		print("		}", file = f)
		print("	}", file = f)
		print("	return NULL;", file = f)
		print("}", file = f)
		print(file = f)

	def _write_source(self, f):
		print("#include <stdio.h>", file = f)
		print("#include \"iomux_pinmap.h\"", file = f)
		print(file = f)
		self._write_array(f, sorted(self._inputs.values()), "input_pins", "IOMUX_INPUT_COUNT")
		self._write_array(f, sorted(self._outputs.values()), "output_pins", "IOMUX_OUTPUT_COUNT")
		self._write_accessor(f, "input", "IOMUX_INPUT_COUNT")
		self._write_accessor(f, "output", "IOMUX_OUTPUT_COUNT")

	def write_header(self, filename):
		with open(filename, "w") as f:
			self._write_header(f)

	def write_source(self, filename):
		with open(filename, "w") as f:
			self._write_source(f)

pinmap = IOMuxPinmap("iomux.txt")
pinmap.write_header("iomux_pinmap.h")
pinmap.write_source("iomux_pinmap.c")
