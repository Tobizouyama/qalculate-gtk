/*
    Qalculate (GTK UI)

    Copyright (C) 2024  Hanna Knutsson (hanna.knutsson@protonmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/

#ifndef QALCULATE_GTK_UTIL_H
#define QALCULATE_GTK_UTIL_H

#include <libqalculate/qalculate.h>
#include <gtk/gtk.h>

enum {
	DELIMITER_COMMA,
	DELIMITER_TABULATOR,
	DELIMITER_SEMICOLON,
	DELIMITER_SPACE,
	DELIMITER_OTHER
};

enum {
	COMMAND_FACTORIZE,
	COMMAND_EXPAND_PARTIAL_FRACTIONS,
	COMMAND_EXPAND,
	COMMAND_TRANSFORM,
	COMMAND_CONVERT_UNIT,
	COMMAND_CONVERT_STRING,
	COMMAND_CONVERT_BASE,
	COMMAND_CONVERT_OPTIMAL,
	COMMAND_CALCULATE,
	COMMAND_EVAL
};

bool string_is_less(std::string str1, std::string str2);
extern KnownVariable *vans[5], *v_memory;
extern MathFunction *f_answer;

struct tree_struct {
	std::string item;
	std::list<tree_struct> items;
	std::list<tree_struct>::iterator it;
	std::list<tree_struct>::reverse_iterator rit;
	std::vector<void*> objects;
	tree_struct *parent;
	void sort() {
		items.sort();
		for(std::list<tree_struct>::iterator it = items.begin(); it != items.end(); ++it) {
			it->sort();
		}
	}
	bool operator < (const tree_struct &s1) const {
		return string_is_less(item, s1.item);
	}
};

#define EXPAND_TO_ITER(model, view, iter)		GtkTreePath *path = gtk_tree_model_get_path(model, &iter); \
							gtk_tree_view_expand_to_path(GTK_TREE_VIEW(view), path); \
							gtk_tree_path_free(path);
#define SCROLL_TO_ITER(model, view, iter)		GtkTreePath *path2 = gtk_tree_model_get_path(model, &iter); \
							gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(view), path2, NULL, FALSE, 0, 0); \
							gtk_tree_path_free(path2);
#define EXPAND_ITER(model, view, iter)			GtkTreePath *path = gtk_tree_model_get_path(model, &iter); \
							gtk_tree_view_expand_row(GTK_TREE_VIEW(view), path, FALSE); \
							gtk_tree_path_free(path);

#define VERSION_BEFORE(i1, i2, i3) (version_numbers[0] < i1 || (version_numbers[0] == i1 && (version_numbers[1] < i2 || (version_numbers[1] == i2 && version_numbers[2] < i3))))
#define VERSION_AFTER(i1, i2, i3) (version_numbers[0] > i1 || (version_numbers[0] == i1 && (version_numbers[1] > i2 || (version_numbers[1] == i2 && version_numbers[2] > i3))))

extern GtkWidget *mainwindow;
extern GtkWidget *expressiontext;
extern GtkTextBuffer *expressionbuffer;

#if GTK_MAJOR_VERSION > 3 || GTK_MINOR_VERSION >= 18
#	define CLEAN_MODIFIERS(x) (x & gdk_keymap_get_modifier_mask(gdk_keymap_get_for_display(gtk_widget_get_display(mainwindow)), GDK_MODIFIER_INTENT_DEFAULT_MOD_MASK))
#else
#	define CLEAN_MODIFIERS(x) (x & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK | GDK_SUPER_MASK | GDK_HYPER_MASK | GDK_META_MASK))
#endif
#ifdef _WIN32
#	define FIX_ALT_GR if(state & GDK_MOD1_MASK && state & GDK_MOD2_MASK && state & GDK_CONTROL_MASK) state &= ~GDK_CONTROL_MASK;
#else
#	define FIX_ALT_GR
#endif

#if GTK_MAJOR_VERSION <= 3 && GTK_MINOR_VERSION < 20
#	define SET_FOCUS_ON_CLICK(x) gtk_button_set_focus_on_click(GTK_BUTTON(x), FALSE)
#else
#	define SET_FOCUS_ON_CLICK(x) gtk_widget_set_focus_on_click(GTK_WIDGET(x), FALSE)
#endif
#define CHILDREN_SET_FOCUS_ON_CLICK(x) list = gtk_container_get_children(GTK_CONTAINER(gtk_builder_get_object(main_builder, x))); \
	for(l = list; l != NULL; l = l->next) { \
		SET_FOCUS_ON_CLICK(l->data); \
	} \
	g_list_free(list);
#define CHILDREN_SET_FOCUS_ON_CLICK_2(x, y) list = gtk_container_get_children(GTK_CONTAINER(gtk_builder_get_object(main_builder, x))); \
	obj = gtk_builder_get_object(main_builder, y); \
	for(l = list; l != NULL; l = l->next) { \
		if(l->data != obj) SET_FOCUS_ON_CLICK(l->data); \
	} \
	g_list_free(list);

#define FIX_SUPSUB_PRE_W(w_supsub) FIX_SUPSUB_PRE(w_supsub, test_supsub(w_supsub))

#define FIX_SUPSUB_PRE(w_supsub, b) \
	string s_sup, s_sub;\
	if(b) {\
		if(pango_version() >= 15000) {\
			s_sup = "<span size=\"60%\" baseline_shift=\"superscript\">";\
			s_sub = "<span size=\"60%\" baseline_shift=\"subscript\">";\
		} else {\
			PangoFontDescription *font_supsub;\
			gtk_style_context_get(gtk_widget_get_style_context(w_supsub), GTK_STATE_FLAG_NORMAL, GTK_STYLE_PROPERTY_FONT, &font_supsub, NULL);\
			s_sup = "<span size=\"x-small\" rise=\""; s_sup += i2s(pango_font_description_get_size(font_supsub) * 0.5); s_sup += "\">";\
			s_sub = "<span size=\"x-small\" rise=\"-"; s_sub += i2s(pango_font_description_get_size(font_supsub) * 0.2); s_sub += "\">";\
		}\
	}

#define FIX_SUPSUB(str) \
	if(!s_sup.empty()) {\
		gsub("<sup>", s_sup, str);\
		gsub("</sup>", "</span>", str);\
		gsub("<sub>", s_sub, str);\
		gsub("</sub>", "</span>", str);\
	}

#define RUNTIME_CHECK_GTK_VERSION(x, y) (gtk_get_minor_version() >= y)
#define RUNTIME_CHECK_GTK_VERSION_LESS(x, y) (gtk_get_minor_version() < y)

#define EQUALS_IGNORECASE_AND_LOCAL(x,y,z)	(equalsIgnoreCase(x, y) || equalsIgnoreCase(x, z))
#define EQUALS_IGNORECASE_AND_LOCAL_NR(x,y,z,a)	(equalsIgnoreCase(x, y a) || (x.length() == strlen(z) + strlen(a) && equalsIgnoreCase(x.substr(0, x.length() - strlen(a)), z) && equalsIgnoreCase(x.substr(x.length() - strlen(a)), a)))

extern tree_struct function_cats, unit_cats, variable_cats;
extern std::string volume_cat;
extern std::vector<std::string> alt_volcats;
extern std::vector<void*> ia_units, ia_variables, ia_functions;
extern std::vector<Unit*> user_units;
extern std::vector<Variable*> user_variables;
extern std::vector<MathFunction*> user_functions;

extern bool b_busy, b_busy_command, b_busy_result, b_busy_expression, b_busy_fetch;

void block_calculation();
void unblock_calculation();
bool calculation_blocked();
void block_error();
void unblock_error();
bool error_blocked();
void block_result();
void unblock_result();
bool result_blocked();

void mainwindow_cursor_moved();

GtkBuilder *getBuilder(const char *filename);

std::string unformat(std::string str);

void get_image_blank_width(cairo_surface_t *surface, int *x1, int *x2);
void get_image_blank_height(cairo_surface_t *surface, int *y1, int *y2);

void set_tooltips_enabled(GtkWidget *w, bool b);
void update_tooltips_enabled();

gchar *font_name_to_css(const char *font_name, const char *w = "*");

std::string unhtmlize(std::string str, bool b_ascii = false);

bool last_is_operator(std::string str, bool allow_exp = false);

const char *sub_sign();
const char *times_sign(bool unit_expression = false);
const char *divide_sign();
bool result_is_autocalculated();

std::string print_with_evalops(const Number &nr);

gint string_sort_func(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data);
gint category_sort_func(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data);
gint int_string_sort_func(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data);
gint compare_categories(gconstpointer a, gconstpointer b);

bool can_display_unicode_string_function(const char *str, void *w);
bool can_display_unicode_string_function_exact(const char *str, void *w);

std::string localize_expression(std::string str, bool unit_expression = false);
std::string unlocalize_expression(std::string str);

void base_from_string(std::string str, int &base, Number &nbase, bool input_base = false);
std::string get_value_string(const MathStructure &mstruct_, int type = 0, Prefix *prefix = NULL);

bool entry_in_quotes(GtkEntry *w);
const gchar *key_press_get_symbol(GdkEventKey *event, bool do_caret_as_xor = true, bool unit_expression = false);

void on_variable_edit_entry_name_changed(GtkEditable *editable, gpointer user_data);

enum {
	PREFIX_MODE_NO_PREFIXES,
	PREFIX_MODE_SELECTED_UNITS,
	PREFIX_MODE_CURRENCIES,
	PREFIX_MODE_ALL_UNITS
};

#ifdef __cplusplus
extern "C" {
#endif
bool do_shortcut(int type, std::string value);
gboolean on_math_entry_key_press_event(GtkWidget *o, GdkEventKey *event, gpointer);
gboolean on_unit_entry_key_press_event(GtkWidget *o, GdkEventKey *event, gpointer);
void entry_insert_text(GtkWidget *w, const gchar *text);
void set_input_base(int base, bool opendialog = false, bool recalculate = true);
void set_output_base(int base);
bool textview_in_quotes(GtkTextView *w);
bool contains_polynomial_division(MathStructure &m);
bool contains_imaginary_number(MathStructure &m);
bool contains_rational_number(MathStructure &m);
bool contains_fraction(MathStructure &m, bool in_div = false);
bool contains_convertible_unit(MathStructure &m);
bool has_prefix(const MathStructure &m);

void minimal_mode_show_resultview(bool b = true);

void stop_autocalculate_history_timeout();

void keypad_font_modified();
void update_app_font(bool initial = false);
void update_colors(bool initial = false);
void set_app_operator_symbols();

void memory_recall();
void memory_store();
void memory_add();
void memory_subtract();
void memory_clear();

void set_fraction_format(int nff);
void set_fixed_fraction(long int v, bool combined);
void set_min_exp(int min_exp);
void set_prefix_mode(int i);
void set_approximation(ApproximationMode approx);
void set_angle_unit(AngleUnit au);
void set_custom_angle_unit(Unit *u);
void set_autocalculate(bool b);
void update_exchange_rates();
void import_definitions_file();
void show_about();
void report_bug();
void set_unknowns();
void open_convert_number_bases();
void open_convert_floatingpoint();
void open_percentage_tool();
void open_calendarconversion();
void show_unit_conversion();
void open_plot();
bool qalculate_quit();

void clear_parsed_in_result();
void show_parsed_in_result(MathStructure &mparse, const PrintOptions &po);
#ifdef __cplusplus
}
#endif
bool test_supsub(GtkWidget *w);
bool use_keypad_buttons_for_history();
bool keypad_is_visible();
bool update_window_title(const char *str = NULL, bool is_result = false);
void set_rpn_mode(bool b);
void set_parsed_in_result(bool b);
void set_minimal_mode(bool b);
void check_for_new_version(bool do_not_show_again = false);
void insert_matrix(const MathStructure *initial_value = NULL, GtkWidget *win = NULL, gboolean create_vector = FALSE, bool is_text_struct = false, bool is_result = false, GtkEntry *entry = NULL);

void on_abort_display(GtkDialog*, gint, gpointer);
void on_abort_command(GtkDialog*, gint, gpointer);
void on_abort_calculation(GtkDialog*, gint, gpointer);

void show_message(const gchar *text, GtkWidget *win);
bool ask_question(const gchar *text, GtkWidget *win);
void show_notification(std::string text);

void update_persistent_keypad(bool showhide_buttons = false);

void set_clipboard(std::string str, int ascii, bool html, bool is_result, int copy_without_units = -1);

void result_format_updated();
void result_action_executed();
void result_prefix_changed(Prefix *prefix = NULL);
void expression_calculation_updated();
void expression_format_updated(bool recalculate = false);
void execute_expression(bool force = true, bool do_mathoperation = false, MathOperation op = OPERATION_ADD, MathFunction *f = NULL, bool do_stack = false, size_t stack_index = 0, std::string execute_str = std::string(), std::string str = std::string(), bool check_exrates = true);
void executeCommand(int command_type, bool show_result = true, bool force = false, std::string ceu_str = "", Unit *u = NULL, int run = 1);
void calculateRPN(int op);
void calculateRPN(MathFunction *f);
bool do_chain_mode(const gchar *op);
void do_auto_calc(int recalculate = 1, std::string str = std::string());
void abort_calculation();
void setResult(Prefix *prefix = NULL, bool update_history = true, bool update_parse = false, bool force = false, std::string transformation = "", size_t stack_index = 0, bool register_moved = false, bool supress_dialog = false);
void convert_result_to_unit_expression(std::string str);
bool display_errors(GtkWidget *win = NULL, int type = 0, bool add_to_history = false);
void add_as_variable();
void add_autocalculated_result_to_history();
bool autocalculation_stopped_at_operator();
void handle_expression_modified(bool autocalc);

void copy_result(int ascii = -1, int type = 0);

void set_expression_output_updated(bool);

void update_message_print_options();

void toggle_binary_pos(int pos);

MathStructure *current_result();
void replace_current_result(MathStructure*);
MathStructure *current_parsed_result();
const std::string &current_result_text();
bool current_result_text_is_approximate();
void clearresult();
void clear_parsed_in_result();

void update_vmenu(bool update_compl = true);
void update_fmenu(bool update_compl = true);
void update_umenus(bool update_compl = true);
bool is_answer_variable(Variable *v);

void variable_removed(Variable *v);
void unit_removed(Unit *u);
void function_removed(MathFunction *f);

void variable_edited(Variable *v);
void function_edited(MathFunction *f);
void unit_edited(Unit *u);
void dataset_edited(DataSet *ds);

void function_inserted(MathFunction *object);

bool equalsIgnoreCase(const std::string &str1, const std::string &str2, size_t i2, size_t i2_end, size_t minlength);
bool title_matches(ExpressionItem *item, const std::string &str, size_t minlength = 0);
bool name_matches(ExpressionItem *item, const std::string &str);
bool country_matches(Unit *u, const std::string &str, size_t minlength = 0);

void insert_variable(Variable *v, bool add_to_menu = true);
void insert_unit(Unit *u, bool add_to_recent = false);
void insert_button_function(MathFunction *f, bool save_to_recent = false, bool apply_to_stack = true);

void apply_function(MathFunction *f);

bool check_exchange_rates(GtkWidget *win = NULL, bool set_result = false);

void convert_result_to_unit(Unit *u);

void find_matching_units(const MathStructure &m, const MathStructure *mparse, std::vector<Unit*> &v, bool top = true);
Unit *find_exact_matching_unit(const MathStructure &m);

long int get_fixed_denominator_gtk(const std::string &str, int &to_fraction, bool qalc_command = false);

void fix_deactivate_label_width(GtkWidget *w);

gboolean on_activate_link(GtkLabel*, gchar *uri, gpointer);

std::string shortcut_to_text(guint key, guint state);
const gchar *shortcut_type_text(int type, bool return_null = false);
std::string button_valuetype_text(int type, const std::string &value);
std::string shortcut_types_text(const std::vector<int> &type);
const char *shortcut_copy_value_text(int v);
std::string shortcut_values_text(const std::vector<std::string> &value, const std::vector<int> &type);
void update_accels(int type = -1);
void update_custom_buttons(int index = -1);

void update_window_properties(GtkWidget *w, bool ignore_tooltips_setting = false);

#define MENU_ITEM_WITH_INT(x,y,z)		item = gtk_menu_item_new_with_label(x); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), GINT_TO_POINTER(z)); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM_WITH_STRING(x,y,z)		item = gtk_menu_item_new_with_label(x); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), (gpointer) z); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM_WITH_POINTER(x,y,z)		item = gtk_menu_item_new_with_label(x); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), (gpointer) z); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM_WITH_OBJECT(x,y)		item = gtk_menu_item_new_with_label(x->title(true, printops.use_unicode_signs, &can_display_unicode_string_function, (void*) sub).c_str()); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), (gpointer) x); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM_WITH_POINTER_PREPEND(x,y,z)	item = gtk_menu_item_new_with_label(x); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), (gpointer) z); gtk_menu_shell_prepend(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM_WITH_OBJECT_PREPEND(x,y)	item = gtk_menu_item_new_with_label(x->title(true, printops.use_unicode_signs, &can_display_unicode_string_function, (void*) sub).c_str()); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), (gpointer) x); gtk_menu_shell_prepend(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM_WITH_OBJECT_AND_FLAG(x,y)	{GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6); unordered_map<string, cairo_surface_t*>::const_iterator it_flag = flag_surfaces.find(x->referenceName()); GtkWidget *image_w; if(it_flag != flag_surfaces.end()) {image_w = gtk_image_new_from_surface(it_flag->second);} else {image_w = gtk_image_new();} gtk_widget_set_size_request(image_w, flagheight * 2, flagheight); gtk_container_add(GTK_CONTAINER(box), image_w); gtk_container_add(GTK_CONTAINER(box), gtk_label_new(x->title(true, printops.use_unicode_signs, &can_display_unicode_string_function, (void*) sub).c_str())); item = gtk_menu_item_new(); gtk_container_add(GTK_CONTAINER(item), box); gtk_widget_show_all(item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), (gpointer) x); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);}
#define MENU_ITEM_MARKUP_WITH_INT_AND_FLAGIMAGE(x,y,z,i)	{GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6); GtkWidget *image_w = gtk_image_new_from_surface(y); gtk_widget_set_size_request(image_w, flagheight * 2, flagheight); gtk_container_add(GTK_CONTAINER(box), image_w); GtkWidget *label = gtk_label_new(x); gtk_container_add(GTK_CONTAINER(box), label); item = gtk_menu_item_new(); gtk_container_add(GTK_CONTAINER(item), box); gtk_widget_show_all(item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(z), GINT_TO_POINTER(i)); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);}
#define MENU_ITEM_MARKUP_WITH_POINTER(x,y,z)	item = gtk_menu_item_new_with_label(""); gtk_label_set_markup(GTK_LABEL(gtk_bin_get_child(GTK_BIN(item))), x); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), (gpointer) z); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM_MARKUP_WITH_INT(x,y,z)	item = gtk_menu_item_new_with_label(""); gtk_label_set_markup(GTK_LABEL(gtk_bin_get_child(GTK_BIN(item))), x); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), GINT_TO_POINTER(z)); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM(x,y)				item = gtk_menu_item_new_with_label(x); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), NULL); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM_MARKUP(x,y)			item = gtk_menu_item_new_with_label(""); gtk_label_set_markup(GTK_LABEL(gtk_bin_get_child(GTK_BIN(item))), x); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), NULL); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_NO_ITEMS(x)			item = gtk_menu_item_new_with_label(x); gtk_widget_set_sensitive(item, FALSE); gtk_widget_show (item); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define CHECK_MENU_ITEM(x,y,b)			item = gtk_check_menu_item_new_with_label(x); gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), b); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), NULL); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define RADIO_MENU_ITEM(x,y,b)			item = gtk_radio_menu_item_new_with_label(group, x); group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item)); gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), b); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), NULL); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define RADIO_MENU_ITEM_WITH_INT(x,y,b,z)	item = gtk_radio_menu_item_new_with_label(group, x); group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item)); gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), b); gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), GINT_TO_POINTER(z)); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define POPUP_CHECK_MENU_ITEM_WITH_LABEL(y,w,l)	item = gtk_check_menu_item_new_with_label(l); gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(w))); gstr = gtk_widget_get_tooltip_text(GTK_WIDGET(w)); if(gstr) {gtk_widget_set_tooltip_text(item, gstr); g_free(gstr);} gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), NULL); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define POPUP_CHECK_MENU_ITEM(y,w)		item = gtk_check_menu_item_new_with_label(gtk_menu_item_get_label(GTK_MENU_ITEM(w))); gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(w))); gstr = gtk_widget_get_tooltip_text(GTK_WIDGET(w)); if(gstr) {gtk_widget_set_tooltip_text(item, gstr); g_free(gstr);} gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), NULL); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define POPUP_RADIO_MENU_ITEM(y,w)		item = gtk_radio_menu_item_new_with_label(group, gtk_menu_item_get_label(GTK_MENU_ITEM(w))); group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item)); gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(w))); gstr = gtk_widget_get_tooltip_text(GTK_WIDGET(w)); if(gstr) {gtk_widget_set_tooltip_text(item, gstr); g_free(gstr);} gtk_widget_show (item); g_signal_connect(G_OBJECT (item), "activate", G_CALLBACK(y), NULL); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_ITEM_SET_ACCEL(a)			gtk_widget_add_accelerator(item, "activate", accel_group, a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
#define MENU_TEAROFF				item = gtk_tearoff_menu_item_new(); gtk_widget_show (item); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_SEPARATOR				item = gtk_separator_menu_item_new(); gtk_widget_show (item); gtk_menu_shell_append(GTK_MENU_SHELL(sub), item);
#define MENU_SEPARATOR_PREPEND			item = gtk_separator_menu_item_new(); gtk_widget_show (item); gtk_menu_shell_prepend(GTK_MENU_SHELL(sub), item);
#define SUBMENU_ITEM(x,y)			item = gtk_menu_item_new_with_label(x); gtk_widget_show (item); gtk_menu_shell_append(GTK_MENU_SHELL(y), item); sub = gtk_menu_new(); gtk_widget_show (sub); gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), sub);
#define SUBMENU_ITEM_PREPEND(x,y)		item = gtk_menu_item_new_with_label(x); gtk_widget_show (item); gtk_menu_shell_prepend(GTK_MENU_SHELL(y), item); sub = gtk_menu_new(); gtk_widget_show (sub); gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), sub);
#define SUBMENU_ITEM_INSERT(x,y,i)		item = gtk_menu_item_new_with_label(x); gtk_widget_show (item); gtk_menu_shell_insert(GTK_MENU_SHELL(y), item, i); sub = gtk_menu_new(); gtk_widget_show (sub); gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), sub);

#endif /* QALCULATE_GTK_UTIL_H */
