#ifndef RES_H
#define RES_H

#define STR_RES(text) QString::fromUtf8(text)


#define S_DATA_GROUP                STR_RES("данные")
#define S_TEST_GROUP                STR_RES("тесты")
#define S_RESULT_GROUP              STR_RES("результаты")

#define S_DATAGROUP_NOT_SELECTED    STR_RES("Группа данных не выбрана")
#define S_GROUP_NOT_SELECTED        STR_RES("Группа не выбрана")
#define S_SAMPLES_NOT_SELECTED      STR_RES("Примеры не выбраны")


/*#define S_PROP_NOT_SELECTED         STR_RES("Свойство не выбрано")
#define S_GROUP_NOT_SELECTED        STR_RES("Группа не выбрана")
#define S_APPLYMODE_NOT_SELECTED    STR_RES("Режим применения не выбран")
#define S_FIELD_IS_EMPTY            STR_RES("Поле не заполнено")
#define S_ROW_NOT_SELECTED          STR_RES("Строка не выбрана")

#define S_ACCEPTED                  STR_RES("Успешно")
#define S_REJECTED                  STR_RES("Неуспешно")

//Всплывающие подсказки над кнопками
#define S_BT_INSERT                 STR_RES("Добавить запись")
#define S_BT_APPLY                  STR_RES("Сохранить изменения")
#define S_BT_CANCEL                 STR_RES("Отменить изменения")
#define S_BT_IMPORT_RECORD          STR_RES("Прикрепить ранее созданную запись")
#define S_BT_ADD_COUPON             STR_RES("Прикрепить талон доступа к мед. записи")
#define S_BT_SIGNS                  STR_RES("Просмотреть подписи и/или\n подписать мед. запись")

#define S_BT_CLOSE                  STR_RES("Закрыть")
#define S_BT_SIGN                   STR_RES("Подписать после аутентификации")
#define S_BT_SIGN_OWN               STR_RES("Подписать собственноручно")

#define S_BT_DELETE_PATIENT         STR_RES("Удалить мед.карту пациента")
#define S_BT_INSERT_PATIENT         STR_RES("Добавить мед.карту пациента в открытом режиме")
#define S_BT_INSERT_ANONYMOUS       STR_RES("Добавить мед.карту анонимного пациента")
#define S_BT_UPDATE_PATIENT         STR_RES("Изменить персональные данные\n/обезличить/раскрыть обезличенность")
#define S_BT_INFO_PATIENT           STR_RES("Просмотреть мед.карту пациента")

#define S_MED_RECORDS               STR_RES("Медицинские записи пациента")

#define S_BT_USER_ACCOUNT           STR_RES("Управление учетной записью")
#define S_BT_LOAD_AUDIT             STR_RES("Выбрать события аудита и добавить к имеющимся")
#define S_BT_CLEAR_AUDIT            STR_RES("Очистить события аудита")

//main.cpp
#define S_CANT_LOAD_BIOGARANT       STR_RES("Модуль БиоГарант не загружен")
#define S_CANT_CONNECT_TO_DB        STR_RES("Ошибка подключения к базе данных: %1")
#define S_CANT_CHANGE_PASSWORD_OTHER  STR_RES("Изменить пароль/способ можно только для своей учетной записи")

//db.cpp
#define S_GARANT                    STR_RES("Гарант")
#define S_ADMIN                     STR_RES("Администратор")
#define S_PATNO_X                   STR_RES("Пац.%1")
#define S_TERMINAL_X                STR_RES("Терминал %1")
#define S_ANONYMOUS                 STR_RES("Аноним")
#define S_REGISTRATION_ERROR        STR_RES("Ошибка регистрации пользователя в БД МИС")
#define S_AUTH_NEEDED               STR_RES("Необходимо аутентифицироваться")
#define S_AUTH2_NEEDED              STR_RES("Необходима аутентификация\n контролирующего лица")
#define S_AUTH_REJECTED             STR_RES("Личность не подтверждена")
#define S_WRONG_PASSWORD            STR_RES("Введен неправильный пароль доступа к МИС")

#define S_USER_LOCKED               STR_RES("Учетная запись заблокирована")
#define S_USER_UNLOCKED             STR_RES("Учетная запись разблокирована")
#define S_USER_REMOVED              STR_RES("Учетная запись удалена")
#define S_USER_CREATED              STR_RES("Учетная запись создана")
#define S_USER_UNKNOWN              STR_RES("Учетная запись не найдена")
#define S_TERM_UNKNOWN              STR_RES("Неизвестный терминал")
#define S_INSUFFICIENT_RIGHTS       STR_RES("Недостаточно прав доступа")

#define S_0RC0                      STR_RES(" р/с ")
#define S_0BIC0                     STR_RES(" БИК ")
#define S_0KC0                      STR_RES(" к/с ")

#define S_0SERIAL0                  STR_RES(" серия ")
#define S_0NO0                      STR_RES(" № ")
#define S_0WHOOM0                   STR_RES(" выдан ")

#define S_0TEL0                     STR_RES(" тел.: ")
#define S_0EMAIL0                   STR_RES(" эл.почта: ")
#define S_0NETS0                    STR_RES(" соц.сети: ")
#define S_0WEB0                     STR_RES(" сайт: ")


#define S_CERT                      STR_RES("сертификат")
#define S_PASSPORT                  STR_RES("паспорт")
#define S_SVIDVO                    STR_RES("свид-во")
#define S_RIGHTS                    STR_RES("права")
#define S_POLIC                     STR_RES("полис")
#define S_MILTICKET                 STR_RES("воен. билет")

#define S_MAN_SEX                   STR_RES("мужской")
#define S_WOMAN_SEX                 STR_RES("женский")

#define S_PUBLIC_REC                STR_RES("открытая")
#define S_PRIVATE_REC               STR_RES("обезличенная")
#define S_ANONYM_REC                STR_RES("анонимная")

#define S_RECEPTION                 STR_RES("прием")
#define S_RECEPTION_CANCELLED       STR_RES("прием отменен")
#define S_PROCEDURES                STR_RES("процедуры")
#define S_FREE_TIME                 STR_RES("нерабочее время")
#define S_LUNCH_BREAK               STR_RES("перерыв")


#define S_ROLE_UNKNOWN              STR_RES("неизвестный")
#define S_ROLE_ADMINISTRATOR        STR_RES("администратор")
#define S_ROLE_REGISTRATOR          STR_RES("регистратор")
#define S_ROLE_DOCTOR               STR_RES("врач")
#define S_ROLE_PATIENT              STR_RES("пациент")
#define S_ROLE_TERMINAL             STR_RES("терминал")
#define S_ROLE_GARANT               STR_RES("гарант")
#define S_ROLE_ANONYMOUS            STR_RES("аноним")

#define S_STATE_DELETED             STR_RES("удалена")
#define S_STATE_ACTIVE              STR_RES("активная")
#define S_STATE_LOCKED              STR_RES("заблокирована")
#define S_STATE_NEW                 STR_RES("новая запись")

#define S_NEW_ACCOUNT               STR_RES("Новая учетная запись")

//dlgclassificator.cpp
#define S_MED_SERVICES_CLASSIF      STR_RES("Классификатор медицинских услуг")
#define S_MED_SPECIALS_CLASSIF      STR_RES("Классификатор медицинских специальностей")
#define S_MED_CLASSES_CLASSIF       STR_RES("Классификатор категорий врачей")
#define S_MED_POSTS_CLASSIF         STR_RES("Классификатор медицинских должностей")

#define S_SERVICE                   STR_RES("Услуга")
#define S_SPECIAL                   STR_RES("Специальность")
#define S_CATEGORY                  STR_RES("Категория")
#define S_CODE                      STR_RES("Название")

#define S_N                         STR_RES("№")
#define S_NAME                      STR_RES("Название")
#define S_LOGIN                     STR_RES("Логин")
#define S_HASH                      STR_RES("Хэш")
#define S_STATE                     STR_RES("Состояние зап.")
#define S_ROLE                      STR_RES("Роль")
#define S_ID                        STR_RES("Идентификатор")
#define S_PDCODE                    STR_RES("Код обезличивания")

//dlgclinic.cpp
#define S_FILIAL                    STR_RES("Филиал")
#define S_INSERT_RECORD             STR_RES("Добавление записи")
#define S_UPDATE_RECORD             STR_RES("Изменение записи")

//dlgcoupon.cpp
#define S_COUPON_NOT_ADDED          STR_RES("Ошибка добавления талона. Проверьте правильность заполнения полей")
#define S_COUPON_ADDED              STR_RES("Талон №%1 создан!")
#define S_COUPON_ADDED_ANONYMOUS    STR_RES("Талон №%1 создан анонимно")


//dlgdepartments.cpp
#define S_DEPARTMENT                STR_RES("Отделение")
#define S_ABOUT                     STR_RES("Сведения")


//dlgemployees.cpp
#define S_PHOTO                     STR_RES("[Фото]")
#define S_TABNO                     STR_RES("Таб.№")
#define S_POST                      STR_RES("Должность")
#define S_ROOM                      STR_RES("Комната")


//dlgemployer.cpp
#define S_IDENTITY_CARD             STR_RES("Удостоверение личности")
#define S_ADDRESS                   STR_RES("Адрес")
#define S_CONTACT_DATA              STR_RES("Контактные данные")
#define S_PERSONAL_BANK_ACCOUNT     STR_RES("Личный счет")

//dlgmedrecords.cpp
#define S_MED_RECORDS_OF_PATIENT_X  STR_RES("Медицинские записи пациента №%1")
#define S_MED_RECORD                STR_RES("Медицинские записи")
#define S_FORM                      STR_RES("Форма")
#define S_AUTHOR                    STR_RES("Автор")
#define S_COUPON                    STR_RES("Талон")
#define S_DATE                      STR_RES("Дата")
#define S_COUPON_FOR_MED_REC_XXXX   STR_RES("Талон на получение\nмед. записи №%1 от %2\nпо форме '%3'\n%4")

#define S_DIGEST_UPDATED            STR_RES("Подпись обновлена")
#define S_DIGEST_NOT_UPDATED        STR_RES("Подпись не обновлена")


//dlgmedrecordsselect.cpp


//dlgmis.cpp
#define S_CONNECTION                STR_RES("Подключение...")
#define S_TERMINAL_CONNECTION       STR_RES("Подключение терминала")

#define S_FIO                       STR_RES("ФИО")
#define S_PATNO                     STR_RES("Пац.№")
#define S_PATIENT_NO                STR_RES("Идентификатор пациента")
#define S_TERMINAL_NO               STR_RES("Номер терминала")
#define S_EMPLOYER_NO               STR_RES("Таб. номер сотрудника")

#define S_RECTYPE                   STR_RES("Тип записи")

#define S_TABLE_CREATED             STR_RES("Таблицы созданы")
#define S_TABLE_NOT_CREATED         STR_RES("Таблицы не созданы")

#define S_TABLE_DELETED             STR_RES("Таблицы удалены")
#define S_TABLE_NOT_DELETED         STR_RES("Таблицы не удалены")

#define S_CARD_NOT_FOUND            STR_RES("Медицинская карта не найдена")

//dlgpatient.cpp
#define S_FIELDS_SKIPPED            STR_RES("Не заполнены обязательные поля формы")
#define S_BINDED_RECORD_NOT_FOUND   STR_RES("Связанная запись не найдена")

#define S_PID_NOT_DEFINED           STR_RES("Персональный идентификатор пациента не определен")
#define S_WRONG_PCODE               STR_RES("Неправильный код обезличивания")
#define S_NO_GARANT_CERTIFICATE     STR_RES("Сертификат гаранта обезличинности не найден")
#define S_SAVE_NEEDED               STR_RES("Необходимо предварительно сохранить введенные данные\nи зарегистрировать учетную запись")
#define S_NO_PCODE                  STR_RES("Ошибка чтения защищенный код обезличивания")

//dlgpatients.cpp
#define S_QUESTION                  STR_RES("Вопрос")
#define S_CREATE_ANONYMOUS          STR_RES("Создать новую анонимную запись пациента?")

//dlgreception.cpp
#define S_TIME                      STR_RES("Время")
#define S_PERSON_INFO               STR_RES("%1\nПолис №%2 от %3\n")
#define S_AUTHENTIFICATION          STR_RES("Подтверждение личности")
#define S_AUTHENTIFICATED           STR_RES("Личность подтверждена")
#define S_NOT_AUTHENTIFICATED       STR_RES("Личность не подтверждена")
#define S_PASSWORD_CHANGING         STR_RES("Смена пароля...")
#define S_PASSWORD_CHANGED          STR_RES("Пароль изменен")

#define S_ADMIN_CREATED             STR_RES("Учетная запись администратора зарегистрирована в БД")
#define S_USER_EXIST                STR_RES("Учетная запись уже существует")
//dlgroom.cpp
//dlgschedule.cpp

#define S_WARNING                   STR_RES("Предупреждение")
#define S_KEYWORDS_NOT_SELECTED     STR_RES("Добавление невозможно. Не выбран врач или дата")
#define S_CANT_RECORD_ON_DATE       STR_RES("Запись невозможна, так как выбрано неприемное время или запись уже занята")
#define S_COUPON_ON_RECEPTION_XXXX  STR_RES("Талон на прием\nспециалист %1\nдата: %2 время: %3\nкомната № %4")

#define S_CONFIRMATION              STR_RES("Подтверждение")
#define S_DELETE_WITHOUT_CONFIRM    STR_RES("Удалить без подтверждения?")

//dlgselectbycoupon.cpp

#define S_INPUT_COUPON_NUMBER_AND_CODE  STR_RES("Введите номер купона и код подтверждения")
#define S_NO_COUPON                 STR_RES("Купон не найден или не совпал код подтверждения")


//dlgsigns.cpp
//..comments
#define S_DOC_SIGNED                STR_RES("Документ подписан")
#define S_SIGNS_OF_DOC_X            STR_RES("Подписи мед. документа %1")
//dlgstruct.cpp

//dlgtypicalforms.cpp
#define S_TYPICAL_MED_FORMS         STR_RES("Типовые медицинские формы")
#define S_TYPICAL_CONTRACT_FORMS    STR_RES("Типовый формы контрактов")

//dlguser.cpp
//..comments

//dlgusername.cpp
//dlguserpatient.cpp

#define S_BANK_ACCOUNT              STR_RES("Счет")
*/

#endif // RES_H
