import random
# https://www.guru99.com/reading-and-writing-files-in-python.html#1

initialCases = [
#Basic Addition
	"i = 1 + 2",
	"i = 1 + 2 + 3",
	"i = 1 + 2 + 3 + 4",
	"i = 1 + 2 + 3 + 4 + 5",
#Addition with Parentheses	
	"i = 1 + 2 + (3 + 4 + 5)",
#

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



# for i in range(len(initialCases)):
# 	# print(initialCases[i])
# 	generateTestFiles(testString = initialCases[i], filename = i)


operators = [
	"+",
	"-",
	"*",
	"/",
	"%"
]


# https://www.pythoncentral.io/how-to-generate-a-random-number-in-python/  random.randint(1,101)
#Valid Expression but invalid results: negative and decimal
#https://pynative.com/python-check-user-input-is-number-or-string/#Python_Program_to_find_user_input_is_a_float_number check float/negative
# Tier 1, 2 operands, for all 5 operators
def generateTier1():

	validCount = 0
	invalidCount = 0

	# Tier one test cases: all expressions are valid, but some results are invalid, i.e. float assignment and negative assignment
	# Expressions includes two operands and one operator

	for j in range(1,10):	

		for i in range(0, len(operators)):
			operandOne = random.randint(0,100)
			operandTwo = random.randint(0,100)

			try:
				results = float(eval(str(operandOne) + operators[i] + str(operandTwo)))
				# print(results)
			except ZeroDivisionError:
				results = 0
				# print("Zero error at" + str(i) + ","  + str(j))
					

			if(float(results) < 0):

				test = str(operandOne) + operators[i] + str(operandTwo)
				generateTestFiles(test, "invalid_output_tier1_" + str(i) + str(j))
				invalidCount = invalidCount + 1 

			elif(float(results).is_integer()):
				
				test = str(operandOne) + operators[i] + str(operandTwo)
				generateTestFiles(test, "valid_output_tier1_" + str(i) + str(j))		
				validCount = validCount + 1 

			else:
				test = str(operandOne) + operators[i] + str(operandTwo)
				generateTestFiles(test, "invalid_output_tier1_" + str(i) + str(j))
				invalidCount = invalidCount + 1 


	# print(validCount)
	# print(invalidCount)

	return;

# generateTier1()




def generateTier2():

	validCount = 0
	invalidCount = 0

	# Tier two test cases: all expressions are valid, but some results are invalid, i.e. float assignment and negative assignment
	# Expressions includes three operands and two operator

	numOp = len(operators)

	for j in range(0,numOp):	

		for i in range(0, numOp ):
			operandOne = random.randint(0,500) # raising the ceiling and floor to increase odds of valid operations
			operandTwo = random.randint(0,100)
			operandThree = random.randint(0,100)

			try:
				results = float(eval(str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree)))
				# print(results)
			except ZeroDivisionError:
				results = 0
				# print("Zero error at" + str(i) + ","  + str(j))
					

			if(float(results) < 0):

				test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree)
				generateTestFiles(test, "invalid_output_tier2_" + str(i) + str(j))
				invalidCount = invalidCount + 1 

			elif(float(results).is_integer()):
				
				test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree)
				generateTestFiles(test, "valid_output_tier2_" + str(i) + str(j))		
				validCount = validCount + 1 

			else:
				test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree)
				generateTestFiles(test, "invalid_output_tier2_" + str(i) + str(j))
				invalidCount = invalidCount + 1 


	# print(validCount)
	# print(invalidCount)

	return;


# generateTier2()




def generateTier3():

	validCount = 0
	invalidCount = 0

	# Tier three test cases: all expressions are valid, but some results are invalid, i.e. float assignment and negative assignment
	# Expressions includes four operands and two operator

	numOp = len(operators)

	for k in range(0,numOp):

		for j in range(0,numOp):	

			for i in range(0, numOp ):
				operandOne = random.randint(0,1000) # raising the ceiling and floor to increase odds of valid operations
				operandTwo = random.randint(0,100)
				operandThree = random.randint(0,100)
				operandFour = random.randint(0,50) # reduce the ceiling to increase odds of valid operations

				try:
					results = float(eval(str(operandOne) + operators[i] 
						+ str(operandTwo) + operators[j] 
						+ str(operandThree) + operators[k]
						+ str(operandFour)
					))
					# print(results)
				except ZeroDivisionError:
					results = 0
					# print("Zero error at" + str(i) + ","  + str(j))
						

				if(float(results) < 0):

					test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree) + operators[k] + str(operandFour)
					generateTestFiles(test, "invalid_output_tier3_" + str(i) + str(j) + str(k))
					invalidCount = invalidCount + 1 

				elif(float(results).is_integer()):
					
					test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree) + operators[k] + str(operandFour)
					generateTestFiles(test, "valid_output_tier3_" + str(i) + str(j) + str(k))		
					validCount = validCount + 1 

				else:
					test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree) + operators[k] + str(operandFour)
					generateTestFiles(test, "invalid_output_tier3_" + str(i) + str(j) + str(k))
					invalidCount = invalidCount + 1 


	# print(validCount)
	# print(invalidCount)

	return;

# generateTier3()



def generateInvalid_doubleOperators():

	numOp = len(operators)


	for j in range(0,numOp):	

		for i in range(0, numOp ):
			operandOne = random.randint(0,1000) # raising the ceiling and floor to increase odds of valid operations
			operandTwo = random.randint(0,100)

			test = str(operandOne) + operators[i] + operators[j] + str(operandTwo) 
			generateTestFiles(test, "invalid_DoubleOperands_" + str(i) + str(j))


	return;

# generateInvalid_doubleOperators()	


def generateInvalid_doubleOperatorsWithSpace():

	numOp = len(operators)


	for j in range(0,numOp):	

		for i in range(0, numOp ):
			operandOne = random.randint(0,1000) # raising the ceiling and floor to increase odds of valid operations
			operandTwo = random.randint(0,100)

			test = str(operandOne) + operators[i] + " " + operators[j] + str(operandTwo) 
			generateTestFiles(test, "invalid_DoubleOperands_Space_" + str(i) + str(j))


	return;

# generateInvalid_doubleOperatorsWithSpace()	




def generateTier4():


	numOp = len(operators)

	for l in range(0, numop):

		for k in range(0,numOp):

			for j in range(0,numOp):	

				for i in range(0, numOp ):
					operandOne = random.randint(0,1000) # raising the ceiling and floor to increase odds of valid operations
					operandTwo = random.randint(0,100)
					operandThree = random.randint(0,100)
					operandFour = random.randint(0,50) # reduce the ceiling to increase odds of valid operations
					operandFive = random.randint(0,10) # reduce the ceiling to increase odds of valid operations

					try:
						results = float(eval(str(operandOne) + operators[i] 
							+ str(operandTwo) + operators[j] 
							+ str(operandThree) + operators[k]
							+ str(operandFour) + operators[l]
							+ str(operandFive)
						))
						# print(results)
					except ZeroDivisionError:
						zeroError = 1

						# print("Zero error at" + str(i) + ","  + str(j))
							
					if(zeroError == 1):

						test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree) + operators[k] + str(operandFour)
						generateTestFiles(test, "invalid_output_tier4_zeroError_" + str(i) + str(j) + str(k))

					elif(float(results).is_integer()):
						
						test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree) + operators[k] + str(operandFour)
						generateTestFiles(test, "valid_output_tier4_" + str(i) + str(j) + str(k))		
					
					elif(float(results) < 0):

						test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree) + operators[k] + str(operandFour)
						generateTestFiles(test, "invalid_output_tier4_" + str(i) + str(j) + str(k))	

					else:
						test = str(operandOne) + operators[i] + str(operandTwo) + operators[j] + str(operandThree) + operators[k] + str(operandFour)
						generateTestFiles(test, "invalid_output_tier4_" + str(i) + str(j) + str(k))


					zeroError = 0

	return;

# generateTier3()

def appendParentheses(expression, check):
	if check == 1:
		return str("(" + expression + ")");
	else:
		return expression;

def betterGenerator():


	# up to 6 operands that will be randomly generator each iterations


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
			fileName = "invalid_output_zero_error_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)
		elif results < 0:
			fileName = "invalid_output_negative_value_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)
		elif not results.is_integer():
			fileName = "invalid_output_float_value_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)		
		elif results.is_integer():
			fileName = "valid_output_" + str(i) + "_num_P_" + str(numParentheses)
			generateTestFiles(resultString, fileName)
		

	return;



betterGenerator()



