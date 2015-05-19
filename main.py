#!/usr/bin/python
import re
student_list = dict()
result_files = {}
result_lines = {}
duplicate = {}
sections = [ "k", "1st", "2nd", "3rd", "4th", "5th", "u1600", "u600", "u300", "mon", "tue", "wed", "thu", "fri"];
result_files["LME"] = "./lme/results.txt"
result_files["Casis"] = "./casis/results.txt"
result_files["Patsy"] = "./patsy/results.txt"

class student:
	first_name = " "
	middle_name = " "
	last_name =" "
	section = "" 
	lme_result = ""
	patsy_result = ""
	casis_result =  ""
	best_result =  ""
	lme_val = "" # float value in string format
	patsy_val = "" # float value in string format 
	casis_val = ""  # float value in string format 
	def __init__(self, section , first_name, last_name , middle_name):
			#print section +  ":"  + first_name + "," + last_name
			self.first_name = first_name
			self.middle_name = middle_name
			self.last_name = last_name
			self.section = section
#--------------------- get list of all student  -------------------
for i in sections:
	f = open("./" + i + ".txt", "r");
	for line in f:	
		words = line.split()
		if len(words) == 3:
			s = student(i,words[0], words[2], words[1])
			student_list.setdefault(i,[]).append(s);
		elif len(words) == 2: 
			s = student(i,words[0], words[1], "")
			student_list.setdefault(i,[]).append(s);
#--------------------- read results  -------------------
for k,v in result_files.items():
	f = open(v)
	for line in f:
		result_lines.setdefault(k,[]).append(line)
#----------------- search name of kid in all 3 result_lines ----------
for k,v in student_list.items():
	for student in v:
		 for place,lines in result_lines.items():
			for line in lines:
					if line.lower().find(student.first_name.lower()) is not -1 and  \
					   line.lower().find(student.last_name.lower()) is not -1 and \
					   line.lower().find(student.middle_name.lower()) is not -1:	
						if place == "LME" :
							student.lme_result = line
						if place == "Patsy" :
							student.patsy_result = line
						if place == "Casis" :
							student.casis_result = line

# ------------- parse results for all kids to get best score --------------
for k,v in student_list.items():
	for s in v:
			if s.casis_result != "":
				match = re.search("\) (n|[0-9])* (\d.\d)", s.casis_result)
				if match:
						s.casis_val =  float(match.group(2)) 
				else: 
					print "ERR" + s.casis_result
			if s.lme_result != "":
				match = re.search("\) (n|[0-9])* (\d.\d)", s.lme_result)
				if match:
						s.lme_val = float(match.group(2))
				else:
					print "ERR" + s.lme_result
			if s.patsy_result != "":
				match = re.search("\) (n|[0-9])* (\d.\d)", s.patsy_result)
				if match:
						s.patsy_val = float(match.group(2))
				else:
					print "ERR" + s.patsy_result
			s.best_val = 0.0
			full_name  = s.first_name + " " + s.last_name
			if duplicate.get(full_name,0)==0:
				duplicate[full_name]=s.section
			else:
				print "Duplicate: ", full_name, " in section : ", duplicate[full_name] , " and " , s.section
			if s.lme_val != "":
				s.best_val = s.lme_val
			if s.casis_val != "" and s.casis_val > s.best_val:
				s.best_val = s.casis_val
			if s.patsy_val != "" and s.patsy_val > s.best_val:
				s.best_val = s.patsy_val
			if not (s.lme_result != "" or s.casis_result != "" or s.patsy_result != ""):
					#print s.section + ", " + s.first_name + ", " + s.last_name + " BEST = ( " + str(s.best_val) + " , LME( " + str(s.lme_val) + " ) CASIS (" + str(s.casis_val) + " ) PATSY ( " + str(s.patsy_val) + " )."
					#print s.section + ", " + str(s.best_val) + " , " + s.first_name + ", "  + s.last_name
					#print s.first_name + ", "+ s.last_name + ", section: " + s.section
					pass
	
