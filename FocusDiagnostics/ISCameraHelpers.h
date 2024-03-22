#pragma once

void print_device(GstDevice* device) {
    GstStructure* struc = gst_device_get_properties(device);

    printf("\tmodel:\t%s\tserial:\t%s\ttype:\t%s\n",
           gst_structure_get_string(struc, "model"),
           gst_structure_get_string(struc, "serial"),
           gst_structure_get_string(struc, "type"));

    gst_structure_free(struc);
}

gboolean bus_function(GstBus* bus __attribute__((unused)), GstMessage* message, gpointer user_data __attribute__((unused))) {
    GstDevice* device;

    switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_DEVICE_ADDED: {
            gst_message_parse_device_added(message, &device);

            printf("NEW device\n");
            print_device(device);
            gst_object_unref(device);
            break;
        }
        case GST_MESSAGE_DEVICE_REMOVED: {
            // this can also be used as an alternative to device-lost signals
            gst_message_parse_device_removed(message, &device);
            printf("REMOVED Device\n");
            print_device(device);
            gst_object_unref(device);
            break;
        }
        default: {
            break;
        }
    }

    // this means we want to continue
    // to listen to device events
    // to stop listening return G_SOURCE_REMOVE;
    return G_SOURCE_CONTINUE;
}

void print_flags(TcamPropertyBase* prop) {
    printf("Available: ");
    GError* err = nullptr;
    gboolean av = tcam_property_base_is_available(prop, &err);
    if (av) printf("yes");
    else printf("no");

    printf("\tLocked: ");
    gboolean isLocked = tcam_property_base_is_locked(prop, &err);
    if (isLocked) printf("yes");
    else printf("no");
}

void list_properties(GstElement* source) {
    GError* err = nullptr;
    GSList* names =  tcam_property_provider_get_tcam_property_names(TCAM_PROPERTY_PROVIDER(source), &err);

    for (unsigned int i = 0; i < g_slist_length(names); ++i) {
        char* name = (char*)g_slist_nth(names, i)->data;
        TcamPropertyBase* base_property = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source), name, &err);
        TcamPropertyType type = tcam_property_base_get_property_type(base_property);

        switch(type) {
            case TCAM_PROPERTY_TYPE_INTEGER: {
                TcamPropertyInteger* integer = TCAM_PROPERTY_INTEGER(base_property);

                gint64 def = tcam_property_integer_get_default(integer, &err);

                gint64 min;
                gint64 max;
                gint64 step;
                tcam_property_integer_get_range(integer, &min, &max, &step, &err);

                if (err) {
                    printf("%s\n", err->message);
                    g_error_free(err);
                    err = nullptr;
                    break;
                }

                gint64 value = tcam_property_integer_get_value(integer, &err);

                if (err) {
                    printf("%s\n", err->message);
                    g_error_free(err);
                    err = nullptr;
                    break;
                }

                const char* unit = "";
                const char* tmp_unit = tcam_property_integer_get_unit(integer);

                if (tmp_unit) unit = tmp_unit;

                printf("%s\ttype: Integer\t"
                       "Display Name: \"%s\" "
                       "Category: %s\n"
                       "\t\t\tDescription: %s\n"
                       "\t\t\tUnit: %s\n"
                       "\t\t\tVisibility: %s\n"
                       "\t\t\tPresentation: %s\n\t\t\t",
                       name,
                       tcam_property_base_get_display_name(base_property),
                       tcam_property_base_get_category(base_property),
                       tcam_property_base_get_description(base_property),
                       unit,
                       g_enum_to_string(tcam_property_visibility_get_type() , tcam_property_base_get_visibility(base_property)),
                       g_enum_to_string(tcam_property_intrepresentation_get_type(), tcam_property_integer_get_representation(integer)));
                print_flags(base_property);
                printf("\n\n"
                       "\t\t\tDefault: %ld\n"
                       "\t\t\tValue: %ld"
                       "\n\n", def, value);

                break;

            }
            case TCAM_PROPERTY_TYPE_FLOAT: {
                TcamPropertyFloat* f = TCAM_PROPERTY_FLOAT(base_property);

                gdouble def = tcam_property_float_get_default(f, &err);

                gdouble min;
                gdouble max;
                gdouble step;
                tcam_property_float_get_range(f, &min, &max, &step, &err);

                if (err) {
                    printf("%s\n", err->message);
                    g_error_free(err);
                    err = nullptr;
                    break;
                }

                gdouble value = tcam_property_float_get_value(f, &err);

                if (err) {
                    printf("%s\n", err->message);
                    g_error_free(err);
                    err = nullptr;
                    break;
                }

                const char* unit = "";
                const char* tmp_unit = tcam_property_float_get_unit(f);

                if (tmp_unit) unit = tmp_unit;

                printf("%s\ttype: Float\t"
                       "Display Name: \"%s\" "
                       "Category: %s\n"
                       "\t\t\tDescription: %s\n"
                       "\t\t\tUnit: %s\n"
                       "\t\t\tVisibility: %s\n"
                       "\t\t\tPresentation: %s\n\t\t\t",
                       name,
                       tcam_property_base_get_display_name(base_property),
                       tcam_property_base_get_category(base_property),
                       tcam_property_base_get_description(base_property),
                       unit,
                       g_enum_to_string(tcam_property_visibility_get_type() , tcam_property_base_get_visibility(base_property)),
                       g_enum_to_string(tcam_property_intrepresentation_get_type(), tcam_property_float_get_representation(f)));
                print_flags(base_property);
                printf("\n\n"
                       "\t\t\tDefault: %f\n"
                       "\t\t\tValue: %f"
                       "\n\n", def, value);

                break;
            }
            case TCAM_PROPERTY_TYPE_ENUMERATION: {
                TcamPropertyEnumeration* e = TCAM_PROPERTY_ENUMERATION(base_property);

                const char* value = tcam_property_enumeration_get_value(e, &err);

                if (err) {
                    printf("%s\n", err->message);
                    g_error_free(err);
                    err = nullptr;
                    break;
                }

                const char* def = tcam_property_enumeration_get_default(e, &err);

                if (err) {
                    printf("%s\n", err->message);
                    g_error_free(err);
                    err = nullptr;
                    break;
                }

                printf("%s\ttype: Enumeration\t"
                       "Display Name: \"%s\" "
                       "Category: %s\n"
                       "\t\t\tDescription: %s\n"
                       "\t\t\tVisibility: %s\n"
                       "\t\t\t",
                       name, tcam_property_base_get_display_name(base_property),
                       tcam_property_base_get_category(base_property),
                       tcam_property_base_get_description(base_property),
                       g_enum_to_string(tcam_property_visibility_get_type() , tcam_property_base_get_visibility(base_property)));
                print_flags(base_property);
                printf("\n\n"
                       "\t\t\tEntries:");

                GSList* enum_entries = tcam_property_enumeration_get_enum_entries(e, &err);

                if (err) {
                    printf("%s\n", err->message);
                    g_error_free(err);
                    break;
                }

                if (enum_entries) {
                    for (GSList* entry = enum_entries; entry != nullptr; entry = entry->next) printf(" %s", (const char*)entry->data);
                    g_slist_free_full(enum_entries, g_free);
                }
                printf("\n\t\t\tDefault: %s\n\t\t\tValue: %s\n\n\n", def, value);

                break;
            }
            case TCAM_PROPERTY_TYPE_BOOLEAN: {
                TcamPropertyBoolean* b = TCAM_PROPERTY_BOOLEAN(base_property);
                gboolean value = tcam_property_boolean_get_value(b, &err);
                gboolean def = tcam_property_boolean_get_default(b, &err);

                if (err) {
                    printf("%s\n", err->message);
                    g_error_free(err);
                    err = nullptr;
                    break;
                }

                const char* val_str = "false";
                const char* def_str = "false";

                if (value) val_str = "true";

                if (def) def_str = "true";

                printf("%s\ttype: Boolean\t"
                       "Display Name: \"%s\" "
                       "Category: %s\n"
                       "\t\t\tDescription: %s\n"
                       "\t\t\t",
                       name,
                       tcam_property_base_get_display_name(base_property),
                       tcam_property_base_get_category(base_property),
                       tcam_property_base_get_description(base_property)
                );
                print_flags(base_property);
                printf("\n\n\t\t\tDefault: %s\n\t\t\tValue: %s\n\n\n", def_str, val_str);

                break;
            }
            case TCAM_PROPERTY_TYPE_COMMAND: {
                printf("%s\ttype: Command\t"
                       "Display Name: \"%s\" "
                       "Category: %s\n"
                       "\t\t\tDescription: %s\n"
                       "\t\t\t",
                       name,
                       tcam_property_base_get_display_name(base_property),
                       tcam_property_base_get_category(base_property),
                       tcam_property_base_get_description(base_property));
                print_flags(base_property);
                printf("\n\n\n");
                break;
            }
            default: {
                break;
            }
        }
        g_object_unref(base_property);
    }
    g_slist_free_full(names, g_free);
}

std::string get_enum_property(GstElement* source, const char* name) {
    std::string value;
    /* this is only a sample not all properties will be set here */
    GError* err = nullptr;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source), name, &err);
    if (err) {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = nullptr;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_ENUMERATION) {
        printf("%s has wrong type. This should not happen.\n", name);
    } else {
        TcamPropertyEnumeration* property_enum = TCAM_PROPERTY_ENUMERATION(property_base);
        const char* out = tcam_property_enumeration_get_value(property_enum, &err);
        value = out;
        if (err) {
            printf("Error while retrieving property: %s\n", err->message);
            g_error_free(err);
            err = nullptr;
        } else {
            printf("%s: %s\n", name, out);
        }
    }
    g_object_unref(property_base);

    return value;
}

void set_enum_property(GstElement* source, const char* name, const char* value) {
    GError* err = nullptr;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source), name, &err);

    if (err) {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = nullptr;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_ENUMERATION) {
        printf("%s has wrong type. This should not happen.\n", name);
    } else {
        TcamPropertyEnumeration* enum_property = TCAM_PROPERTY_ENUMERATION(property_base);
        tcam_property_enumeration_set_value(enum_property, value, &err);
        if (err) {
            printf("Error while setting property: %s\n", err->message);
            g_error_free(err);
            err = nullptr;
        } else {
            printf("Set %s to %s\n", name, value);
        }
    }
    g_object_unref(property_base);
}

double get_float_property(GstElement* source, const char* name) {
    double value = 0.;
    GError* err = nullptr;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source), name, &err);
    if (err) {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = nullptr;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_FLOAT) {
        printf("%s has wrong type. This should not happen.\n", name);
    } else {
        TcamPropertyFloat *property_float = TCAM_PROPERTY_FLOAT(property_base);
        value = tcam_property_float_get_value(property_float, &err);
        if (err) {
            printf("Error while retrieving property: %s\n", err->message);
            g_error_free(err);
            err = nullptr;
        } else {
            printf("%s: %f\n", name, value);
        }
    }
    g_object_unref(property_base);

    return value;
}

void set_float_property(GstElement* source, const char* name, double value) {
    GError* err = nullptr;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source), name, &err);

    if (err) {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = nullptr;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_FLOAT) {
        printf("%s has wrong type. This should not happen.\n", name);
    } else {
        TcamPropertyFloat * float_property = TCAM_PROPERTY_FLOAT(property_base);
        tcam_property_float_set_value(float_property, value, &err);
        if (err) {
            printf("Error while setting property: %s\n", err->message);
            g_error_free(err);
            err = nullptr;
        } else {
            printf("Set %s to %f\n", name, value);
        }
    }
    g_object_unref(property_base);
}

void set_int_property(GstElement* source, const char* name, int value) {
    GError* err = nullptr;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source), name, &err);

    if (err) {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = nullptr;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_INTEGER) {
        printf("%s has wrong type. This should not happen.\n", name);
    } else {
        TcamPropertyInteger * integer_property = TCAM_PROPERTY_INTEGER(property_base);
        tcam_property_integer_set_value(integer_property, value, &err);
        if (err) {
            printf("Error while setting property: %s\n", err->message);
            g_error_free(err);
            err = nullptr;
        } else {
            printf("Set %s to %i\n", name, value);
        }
    }
    g_object_unref(property_base);
}

void set_bool_property(GstElement* source, const char* name, bool value) {
    GError* err = nullptr;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source), name, &err);

    if (err) {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = nullptr;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_BOOLEAN) {
        printf("%s has wrong type. This should not happen.\n", name);
    } else {
        TcamPropertyBoolean * boolean_property = TCAM_PROPERTY_BOOLEAN(property_base);
        tcam_property_boolean_set_value(boolean_property, value, &err);
        if (err) {
            printf("Error while setting property: %s\n", err->message);
            g_error_free(err);
            err = nullptr;
        } else {
            printf("Set %s to %i\n", name, value);
        }
    }
    g_object_unref(property_base);
}

void set_default_values(GstElement* source) {
    //    BlackLevel	type: Float
    //    Display Name: "Black Level"
    //    Category: Exposure
    //    Description: Controls the analog black level as an absolute physical value. This represents a DC offset applied to the video signal.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 16.000000
    //    Value: 16.000000
    set_float_property(source, "BlackLevel", 0);

    //    ExposureAuto	type: Enumeration
    //    Display Name: "Exposure Auto"
    //    Category: Exposure
    //    Description: Sets the automatic exposure control mode.
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Available: yes
    //    Locked: no
    //    Entries: Off Continuous
    //    Default: Continuous
    //    Value: Continuous
    set_enum_property(source, "ExposureAuto", "Off");

    //    ExposureTime	type: Float
    //    Display Name: "Exposure Time"
    //    Category: Exposure
    //    Description: Sets the Exposure time when ExposureMode is Timed and ExposureAuto is Off. This controls the duration where the photosensitive cells are exposed to light.
    //    Unit: µs
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LOGARITHMIC
    //    Available: yes
    //    Locked: yes
    //    Default: 33333.000000
    //    Value: 1000.000000
    set_float_property(source, "ExposureTime", 1000);

    //    ExposureAutoReference	type: Integer
    //    Display Name: "Exposure Auto Reference"
    //    Category: Exposure
    //    Description: Configures the target image brightness of the ExposureAuto/GainAuto algorithm.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 128
    //    Value: 128
    set_int_property(source, "ExposureAutoReference", 128);

    //    ExposureAutoLowerLimit	type: Float
    //    Display Name: "Exposure Auto Lower Limit"
    //    Category: Exposure
    //    Description: Lower limit of the ExposureAuto function.
    //    Unit: µs
    //    Visibility: TCAM_VISIBILITY_GURU
    //    Presentation: TCAM_INTREPRESENTATION_LOGARITHMIC
    //    Available: yes
    //    Locked: no
    //    Default: 100.000000
    //    Value: 100.000000
    set_float_property(source, "ExposureAutoLowerLimit", 100.0);

    //    ExposureAutoUpperLimit	type: Float
    //    Display Name: "Exposure Auto Upper Limit"
    //    Category: Exposure
    //    Description: Upper limit of the ExposureAuto function.
    //    Unit: µs
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LOGARITHMIC
    //    Available: yes
    //    Locked: yes
    //    Default: 1000000.000000
    //    Value: 1000000.000000
    //    CANNOT SET THIS PROPERTY

    //    ExposureAutoUpperLimitAuto	type: Boolean
    //    Display Name: "Exposure Auto Upper Limit Auto"
    //    Category: Exposure
    //    Description: Automatically sets the upper limit to match the Acquisition Frame Rate.
    //    Available: yes
    //    Locked: no
    //    Default: true
    //    Value: true
    set_bool_property(source, "ExposureAutoUpperLimitAuto", true);

    //    ExposureAutoHighlightReduction	type: Boolean
    //    Display Name: "Exposure Auto Highlight Reduction"
    //    Category: Exposure
    //    Description: Lets the ExposureAuto/GainAuto algorithm try to avoid over-exposures.
    //    Available: yes
    //    Locked: no
    //    Default: false
    //    Value: false
    set_bool_property(source, "ExposureAutoHighlightReduction", false);

    //    GainAuto	type: Enumeration
    //    Display Name: "Gain Auto"
    //    Category: Exposure
    //    Description: Sets the automatic gain control (AGC) mode. The exact algorithm used to implement AGC is device-specific.
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Available: yes
    //    Locked: no
    //    Entries: Off Continuous
    //    Default: Continuous
    //    Value: Continuous
    set_enum_property(source, "GainAuto", "Off");

    //    Gain	type: Float
    //    Display Name: "Gain"
    //    Category: Exposure
    //    Description: Controls the selected gain as an absolute physical value. This is an amplification factor applied to the video signal.
    //    Unit: dB
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: yes
    //    Default: 48.000000
    //    Value: 48.000000
    set_float_property(source, "Gain", 48.0);

    //    GainAutoLowerLimit	type: Float
    //    Display Name: "Gain Auto Lower Limit"
    //    Category: Exposure
    //    Description: Lower limit of the GainAuto function.
    //    Unit: dB
    //    Visibility: TCAM_VISIBILITY_GURU
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 48.000000
    //    Value: 48.000000
    set_float_property(source, "GainAutoLowerLimit", 48.0);

    //    GainAutoUpperLimit	type: Float
    //    Display Name: "Gain Auto Upper Limit"
    //    Category: Exposure
    //    Description: Upper limit of the GainAuto function.
    //    Unit: dB
    //    Visibility: TCAM_VISIBILITY_EXPERT
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 2047.000000
    //    Value: 2047.000000
    set_float_property(source, "GainAutoUpperLimit", 2047.0);

    //    TriggerMode	type: Enumeration
    //    Display Name: "Trigger Mode"
    //    Category: Trigger
    //    Description: Controls if the selected trigger is active.
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Available: yes
    //    Locked: no
    //    Entries: Off On
    //    Default: Off
    //    Value: Off
    set_enum_property(source, "TriggerMode", "Off");

    //    TriggerDelay	type: Float
    //    Display Name: "Trigger Delay"
    //    Category: Trigger
    //    Description: Specifies the delay in microseconds (us) to apply after the trigger reception before activating it.
    //    Unit: µs
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 0.000000
    //    Value: 150.000000
    set_float_property(source, "TriggerDelay", 0.0);

    //    StrobeEnable	type: Enumeration
    //    Display Name: "Strobe Enable"
    //    Category: DigitalIO
    //    Description: Enables the strobe controls.
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Available: yes
    //    Locked: no
    //    Entries: Off On
    //    Default: Off
    //    Value: Off
    set_enum_property(source, "StrobeEnable", "Off");

    //    StrobePolarity	type: Enumeration
    //    Display Name: "Strobe Polarity"
    //    Category: DigitalIO
    //    Description: Controls the polarity of the strobe signal.
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Available: yes
    //    Locked: no
    //    Entries: ActiveHigh ActiveLow
    //    Default: ActiveHigh
    //    Value: ActiveHigh
    set_enum_property(source, "StrobePolarity", "ActiveHigh");

    //    StrobeOperation	type: Enumeration
    //    Display Name: "Strobe Operation"
    //    Category: DigitalIO
    //    Description: Specifies how the length of the strobe pulses is controlled.
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Available: yes
    //    Locked: no
    //    Entries: FixedDuration Exposure
    //    Default: FixedDuration
    //    Value: FixedDuration
    set_enum_property(source, "StrobeOperation", "FixedDuration");

    //    GPOut	type: Integer
    //    Display Name: "General Purpose Output"
    //    Category: DigitalIO
    //    Description: Status of the digital output pin.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 0
    //    Value: 0
    set_int_property(source, "GPOut", 0);

    //    GPIn	type: Integer
    //    Display Name: "General Purpose Input"
    //    Category: DigitalIO
    //    Description: Status of the digital input pin.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 0
    //    Value: 0
    set_int_property(source, "GPIn", 0);

    //    OffsetAutoCenter	type: Enumeration
    //    Display Name: "Offset Auto Center"
    //    Category: Partial Scan
    //    Description: Enables automatic centering of the region of interest area to the sensor when using smaller video dimensions.
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Available: yes
    //    Locked: no
    //    Entries: Off On
    //    Default: On
    //    Value: On
    set_enum_property(source, "OffsetAutoCenter", "Off");


    //    OffsetX	type: Integer
    //    Display Name: "Offset X"
    //    Category: Partial Scan
    //    Description: Horizontal offset from the origin to the region of interest (in pixels).
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: yes
    //    Default: 0
    //    Value: 0
    set_int_property(source, "OffsetX", 0);


    //    OffsetY	type: Integer
    //    Display Name: "Offset Y"
    //    Category: Partial Scan
    //    Description: Vertical offset from the origin to the region of interest (in pixels).
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: yes
    //    Default: 0
    //    Value: 0
    set_int_property(source, "OffsetY", 0);

    //    TriggerOperation	type: Enumeration
    //    Display Name: "Trigger Operation"
    //    Category: Trigger
    //    Description: Controls the operation mode of the sensor in trigger mode.
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Available: yes
    //    Locked: no
    //    Entries: Default Global Reset Release
    //    Default: Default
    //    Value: Default
    set_enum_property(source, "TriggerOperation", "Default");

    //    SensorWidth	type: Integer
    //    Display Name: "Sensor Width"
    //    Category: Sensor
    //    Description: Effective width of the sensor in pixels.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_EXPERT
    //    Presentation: TCAM_INTREPRESENTATION_PURENUMBER
    //    Available: yes
    //    Locked: yes
    //    Default: 1280
    //    Value: 1280
    //    Cannot SET this property

    //    SensorHeight	type: Integer
    //    Display Name: "Sensor Height"
    //    Category: Sensor
    //    Description: Effective height of the sensor in pixels.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_EXPERT
    //    Presentation: TCAM_INTREPRESENTATION_PURENUMBER
    //    Available: yes
    //    Locked: yes
    //    Default: 960
    //    Value: 960
    //    Cannot SET this property

    //    AutoFunctionsROIEnable	type: Boolean
    //    Display Name: "Enable Auto Functions ROI"
    //    Category: Auto ROI
    //    Description: Enable the region of interest for auto functions.
    //    Available: yes
    //    Locked: no
    //    Default: false
    //    Value: false
    set_bool_property(source, "AutoFunctionsROIEnable", false);

    //    AutoFunctionsROIPreset	type: Enumeration
    //    Display Name: "Auto Functions ROI Preset"
    //    Category: Auto ROI
    //    Description: Select a predefined region of interest for auto functions
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Available: yes
    //    Locked: no
    //    Entries: Full Sensor Custom Rectangle Center 50% Center 25% Bottom Half Top Half
    //    Default: Full Sensor
    //    Value: Full Sensor
    set_enum_property(source, "AutoFunctionsROIPreset", "Full Sensor");

    //    AutoFunctionsROILeft	type: Integer
    //    Display Name: "Auto Functions ROI Left"
    //    Category: Auto ROI
    //    Description: Horizontal offset of the auto functions region of interest.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 0
    //    Value: 0
    set_int_property(source, "AutoFunctionsROILeft", 0);

    //    AutoFunctionsROITop	type: Integer
    //    Display Name: "Auto Functions ROI Top"
    //    Category: Auto ROI
    //    Description: Vertical offset of the auto functions region of interest.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 0
    //    Value: 0
    set_int_property(source, "AutoFunctionsROITop", 0);

    //    AutoFunctionsROIWidth	type: Integer
    //    Display Name: "Auto Functions ROI Width"
    //    Category: Auto ROI
    //    Description: Horizontal size of the auto functions region of interest.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 0
    //    Value: 0
    set_int_property(source, "AutoFunctionsROIWidth", 0);

    //    AutoFunctionsROIHeight	type: Integer
    //    Display Name: "Auto Functions ROI Height"
    //    Category: Auto ROI
    //    Description: Vertical size of the auto functions region of interest.
    //    Unit:
    //    Visibility: TCAM_VISIBILITY_BEGINNER
    //    Presentation: TCAM_INTREPRESENTATION_LINEAR
    //    Available: yes
    //    Locked: no
    //    Default: 0
    //    Value: 0
    set_int_property(source, "AutoFunctionsROIHeight", 0);
}