/*******************************************************************************
 * Copyright (c) 2012-2017 toem
 *
 * All rights reserved. This source code and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 *******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "flux.h"
#include "math.h"

// geometry
#define MAX_ITEM_ID 3 // maximum id of scope/signal
#define MAX_ENTRY_SIZE 4096

int main() {

	int n;
	int iVal;
	float fVal;

	// output file
	FILE *out = fopen("flux.example.recTr", "wb");

	// calculate required memory for trace and buffers
	unsigned bufferSize = FLX_BUFFER_BYTES(MAX_ENTRY_SIZE);
	unsigned traceSize = FLX_TRACE_BYTES(0, MAX_ITEM_ID);

	// static memory
	unsigned char memoryBuffer[bufferSize];
	unsigned char memoryTrace[traceSize];

	// buffer
	flxBuffer buffer = flxCreateLinearBuffer(memoryBuffer, bufferSize,
			flxWriteToFile, out);

	// trace
	flxTrace trace = flxCreateTrace(0, MAX_ITEM_ID, MAX_ENTRY_SIZE, memoryTrace,
			traceSize, buffer);

	if (trace != 0) {

		// head
		flxAddHead(trace, "example", "flux example");

		// add signals
		// parent 0 is root
//		flxAddSignal(trace, 1, 0, "integer", "an integer", FLX_TYPE_INTEGER, 0);
//		flxAddSignal(trace, 2, 0, "float", "a float", FLX_TYPE_FLOAT, 0);

		// open
		flxOpen(trace, 0, "ns", 0, 0);

		// generate example trace
		for (n = 0; n < 5000000; n++) {

			if (n>0 && !(n%10000)){
				// head
				flxAddModeHead(trace, "example", "flux example",FLX_MODE_HEAD_SYNC);

				// add signals
				// parent 0 is root
//				flxAddSignal(trace, 1, 0, "integer", "an integer", FLX_TYPE_INTEGER, 0);
//				flxAddSignal(trace, 2, 0, "float", "a float", FLX_TYPE_FLOAT, 0);
				flxOpen(trace, n, "ns", 0, 0);
			}
			// time in ns
			flxdomain current = n * 10;

			// integer
			iVal = n % 444;
			flxWriteIntAt(trace, 1, 0, current, 0, &iVal, sizeof(int), 0);

			// float - same time - use domain=0; isDelta=1
			fVal = sin(n / 1000.0);
			flxWriteFloatAt(trace, 2, 0, 0, 1, &fVal, sizeof(float));
		}

		// close
		flxClose(trace, 0, n * 10);
	}

	// flush buffers
	flxFlush(trace);
	fclose(out);
}

#ifdef __cplusplus
}
#endif

