#!/usr/bin/python3

(adu_30, adu_110) = var("adu_30,adu_110")
(adu_vref, adu_temp) = var("adu_vref,adu_temp")

delta_t = 80
vrefint = 121 / 100

v_30 = adu_30 / 0xfff * 33 / 10
v_110 = adu_110 / 0xfff * 33 / 10
v_ref = vrefint * 0xfff / adu_vref
v_temp = adu_temp / 0xfff * v_ref
delta = (v_110 - v_30) / delta_t

print("v_30   = ", v_30)
print("v_110  =", v_110)
print("v_ref  =", v_ref)
print("v_temp =", v_temp)

temperature_tenths = 10 * ((v_temp - v_30) / delta + 30)
print("temperature_tenths_deg_c =", simplify(temperature_tenths))
print(expand(temperature_tenths))

values = {
	"adu_30": 942,
	"adu_110": 1196,
	"adu_vref": 1662,
	"adu_temp": 1064,
}

print(float(v_30.subs(**values)))
print(float(v_110.subs(**values)))
print(float(v_ref.subs(**values)))
print(float(delta.subs(**values)))
print(float(v_temp.subs(**values)))
print(float(temperature_tenths.subs(**values)))
