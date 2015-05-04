package main

import (
	"fmt"
	"io/ioutil"
	"strings"
)

var lme_results []string
var casis_results []string
var patsy_results []string

type student struct {
	f_name string
	m_name string
	l_name string
	best   float32
	lme    string
	casis  string
	patsy  string
}

var students_map map[string][]*student

func open_results(filename string, lines []string) {
	content, err := ioutil.ReadFile(filename)
	if err != nil {
		fmt.Println("file error", filename)
		panic("file error")
	}
	lines = strings.Split(string(content), "\n")
}

func load_students(sections []string) {
	for _, value := range sections {
		file_name := "./" + value + ".txt"
		content, err := ioutil.ReadFile(file_name)
		if err != nil {
			fmt.Println("file error", file_name)
			panic("file error")
		}
		lines := strings.Split(string(content), "\n")
		students_map = make(map[string][]*student)

		for _, line := range lines {
			names := strings.Split(string(line), " ")
			var first_name string
			var mid_name string
			var last_name string
			for key, _ := range names {
				if key == 0 {
					first_name = names[0]
				} else if key == 1 {
					mid_name = names[1]
				} else if key == 3 {
					last_name = names[3]
				}
			}
			s := student{}
			s.f_name = first_name
			s.l_name = mid_name
			s.m_name = last_name

			students_map[value] = make([]*student, 100)
			students_map[value].append(student{first_name, mid_name, last_name, 0})
			fmt.Println(first_name, mid_name, last_name)
		}
	}
}

func main() {
	sections := []string{"k", "1st", "2nd", "3rd", "4th", "5th", "u1600", "u600", "u300", "mon", "tue", "wed", "thu", "fri"}
	open_results("./lme/results.txt", lme_results)
	open_results("./casis/results.txt", casis_results)
	open_results("./patsy/results.txt", patsy_results)
	open_results("./patsy/results.txt", patsy_results)
	load_students(sections)

}
