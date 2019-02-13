import random

operators = [
	"+",
	"-",
	"*",
	"/",
	"%"
]

def generateTestFiles(testString, filename, ):

	filename = str(filename) + ".txt"
	test = "1+1"

	f = open(filename,"w+") 
	f.write("procedure main {\n") # opening line
	# iteration of expressions
	f.write("  ")
	f.write(testString)
	f.write(";\n")

	f.write("}") # ending line

	f.close() 

	return;

def appendParentheses(expression, check):
	if check == 1:
		return str("(" + expression + ")");
	else:
		return expression;

def betterGenerator():


	# up to 6 operands that will be randomly generator each iterations


	numZeroError = 0
	numNegError = 0
	numFloat = 0 
	numValid = 0

	for i in range(0,200):

		zeroError = 0
		invalidSyntax = 0

		operands = [
			random.randint(0,100), 
			random.randint(0,100), 
			random.randint(0,100), 
			random.randint(0,50), 
			random.randint(0,25),
			random.randint(0,10) 
		]

		numParentheses = 0
		# I've created a flag to track the number of parentheses in the SIMPLE source
		appendParenthesesFlag = [
			random.randint(0,1),
			random.randint(0,1),
			random.randint(0,1),
			random.randint(0,1),

		]

		for j in appendParenthesesFlag:
			if appendParenthesesFlag[j] == 1:
				numParentheses = numParentheses + 1	

		resultString = str(
			appendParentheses( # first P
				appendParentheses( # second P
					str(operands[0]) + operators[random.randint(0,4)] + str(operands[1]), 
				appendParenthesesFlag[0]) # end of second P

				+ operators[random.randint(0,4)] + 
				
				appendParentheses( # third P
					str(operands[2]) + operators[random.randint(0,4)] + str(operands[3]), 
				appendParenthesesFlag[1]), # end of third P
			appendParenthesesFlag[2]) # end of first P 
				+ operators[random.randint(0,4)] + 
				appendParentheses( # fourth P
					str(operands[4]) + operators[random.randint(0,4)] + str(operands[4]), 
				appendParenthesesFlag[3]) # end of fourth P
		)


		try:	
			results = float(
					eval(resultString)
				)
		# print(results)
		except ZeroDivisionError:
			zeroError = 1

		# print(results)

		if zeroError == 1:
			pathName = "C:\\00_Main\\2_Side Projects\\01_Python\\00_Test Case Generator\\invalid_output_zero_error\\P_" + str(numParentheses) + "\\"
			fileName = pathName + str(numZeroError)
			numZeroError = numZeroError + 1
			# fileName = "invalid_output_zero_error_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)
		elif results < 0:
			pathName = "C:\\00_Main\\2_Side Projects\\01_Python\\00_Test Case Generator\\invalid_output_negative_value\\P_" + str(numParentheses) + "\\"
			fileName = pathName + str(numNegError)
			numNegError = numNegError + 1
			# fileName = "invalid_output_negative_value_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)
		elif not results.is_integer():
			pathName = "C:\\00_Main\\2_Side Projects\\01_Python\\00_Test Case Generator\\invalid_output_float_value\\P_" + str(numParentheses) + "\\"
			fileName = pathName + str(numFloat)
			numFloat = numFloat + 1
			# fileName = "invalid_output_float_value_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)		
		elif results.is_integer():

			pathName = "C:\\00_Main\\2_Side Projects\\01_Python\\00_Test Case Generator\\valid_output\\P_" + str(numParentheses) + "\\"
			# print(pathName)
			fileName = pathName + str(numValid)
			# print(fileName)
			numValid = numValid + 1
			# fileName = "valid_output_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)
		

	return;


# betterGenerator()



def betterGeneratorv2():


	# up to 6 operands that will be randomly generator each iterations


	numZeroError = 0
	numNegError = 0
	numFloat = 0 
	numValid = 0

	for i in range(0,200):

		zeroError = 0
		invalidSyntax = 0

		operands = [
			random.randint(0,100), 
			random.randint(0,100), 
			random.randint(0,100), 
			random.randint(0,50), 
			random.randint(0,25),
			random.randint(0,10) 
		]

		numParentheses = 0
		# I've created a flag to track the number of parentheses in the SIMPLE source
		appendParenthesesFlag = [
			random.randint(0,1),
			random.randint(0,1),
			random.randint(0,1),
			random.randint(0,1),

		]

		for j in appendParenthesesFlag:
			if appendParenthesesFlag[j] == 1:
				numParentheses = numParentheses + 1	

		resultString = str(
			appendParentheses( # first P
				appendParentheses( # second P
					str(operands[0]) + operators[random.randint(0,4)] + str(operands[1]), 
				appendParenthesesFlag[0]) # end of second P

				+ operators[random.randint(0,4)] + 
				
				appendParentheses( # third P
					str(operands[2]) + operators[random.randint(0,4)] + str(operands[3]), 
				appendParenthesesFlag[1]), # end of third P
			appendParenthesesFlag[2]) # end of first P 
				+ operators[random.randint(0,4)] + 
				appendParentheses( # fourth P
					str(operands[4]) + operators[random.randint(0,4)] + str(operands[4]), 
				appendParenthesesFlag[3]) # end of fourth P
		)


		try:	
			results = float(
					eval(resultString)
				)
		# print(results)
		except ZeroDivisionError:
			zeroError = 1

		# print(results)

		if zeroError == 1:
			pathName = "C:\\00_Main\\2_Side Projects\\01_Python\\00_Test Case Generator\\invalid_output_zero_error\\P_" + str(numParentheses) + "\\"
			fileName = pathName + str(numZeroError)
			numZeroError = numZeroError + 1
			# fileName = "invalid_output_zero_error_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)
		elif results < 0:
			pathName = "C:\\00_Main\\2_Side Projects\\01_Python\\00_Test Case Generator\\invalid_output_negative_value\\P_" + str(numParentheses) + "\\"
			fileName = pathName + str(numNegError)
			numNegError = numNegError + 1
			# fileName = "invalid_output_negative_value_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)
		elif not results.is_integer():
			pathName = "C:\\00_Main\\2_Side Projects\\01_Python\\00_Test Case Generator\\invalid_output_float_value\\P_" + str(numParentheses) + "\\"
			fileName = pathName + str(numFloat)
			numFloat = numFloat + 1
			# fileName = "invalid_output_float_value_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)		
		elif results.is_integer():

			pathName = "C:\\00_Main\\2_Side Projects\\01_Python\\00_Test Case Generator\\valid_output\\P_" + str(numParentheses) + "\\"
			# print(pathName)
			fileName = pathName + str(numValid)
			# print(fileName)
			numValid = numValid + 1
			# fileName = "valid_output_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)
		

	return;


betterGeneratorv2()